#include "api.h"

#include "game_manager.h"
#include "mongoose.h"
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "json_helper.h"
#include "game.h"
#include "log.h"

#define MAX_CONNECTIONS 50

/* Global variables ***********************************************************/

static struct mg_mgr mgr;
static api_conn_t connections[MAX_CONNECTIONS];
static const char* web_root = "www";

/* Function prototypes ********************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

static void new_connection(struct mg_connection* c);
static void del_connection(struct mg_connection* c);

static int get_game_id(struct mg_str query);
static int get_ws_info(struct mg_str data, char* msg);

static void set_game_id(unsigned long c_id, int game_id);

/* Callbacks ******************************************************************/

//TODO check errors of json functions

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data)
{
	game_event_rsp_t game_rsp;
	// dartboard_event_rsp_t dartboard_rsp;

	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message* hm = (struct mg_http_message*) ev_data;
		if (mg_http_match_uri(hm, "/websocket")) {
			// Upgrade to websocket. From now on, a connection is a full-duplex
			// Websocket connection, which will receive MG_EV_WS_MSG events.
			new_connection(c);
			int ws_game_id = get_game_id(hm->query);
			LOG_INFO("WS game ID: %d", ws_game_id);
			LOG_TRACE("SETTING CONN %d TO GAME_ID %d", c->id, ws_game_id);
			set_game_id(c->id, ws_game_id);
			mg_ws_upgrade(c, hm, NULL);
			LOG_DEBUG("Upgrading to websockets!");
		} else if (mg_http_match_uri(hm, "/status")) {
			int game_id = get_game_id(hm->query);
			game_t* game = game_manager_get_by_id(game_id);
			game_event_t event;
			event.type = GAME_EVENT_STATUS;
			game_new_event(game, &event, &game_rsp);
			const char* json = json_helper_simple_str("result",
					game_rsp.ret_str);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/new-dart")) {
			int game_id = get_game_id(hm->query);
			game_t* game = game_manager_get_by_id(game_id);
			int board_id, num, zone;
			json_helper_new_dart(hm->body.ptr, &board_id, &num, &zone);
			game_event_t event;
			event.type = GAME_EVENT_NEW_DART;
			event.dart.number = num;
			event.dart.zone = zone;
			game_new_event(game, &event, &game_rsp);
			const char* json = json_helper_simple_str("result",
					game_rsp.ret_str);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/next-player")) {
			int game_id = get_game_id(hm->query);
			game_t* game = game_manager_get_by_id(game_id);
			game_event_t event;
			event.type = GAME_EVENT_NEXT_PLAYER;
			game_new_event(game, &event, &game_rsp);
			const char* json = json_helper_simple_str("result",
					game_rsp.ret_str);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/new-game")) {
			int game_ref;
			int options;
			char* players[20];
			int n_players;
			json_helper_new_game(hm->body.ptr, &game_ref, &options, players,
					&n_players);
			game_t* game = game_manager_new(game_ref, options);
			assert(game);
			for (int i = 0; i < n_players; i++) {
				game_event_t event;
				event.type = GAME_EVENT_NEW_PLAYER;
				event.player.name = players[i];
				game_new_event(game, &event, &game_rsp);
			}
			game_event_t event;
			event.type = GAME_EVENT_NEW_GAME;
			event.game_id = game_ref; // TODO: game_id -> game_ref o quitarlo
			event.options = options;
			game_new_event(game, &event, &game_rsp);
			// const char* json = json_helper_simple_str("result",
			// 		game_rsp.ret_str);
			const char* json = json_helper_simple_int("game_id",
					game->id);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/back")) {
			int game_id = get_game_id(hm->query);
			game_t* game = game_manager_get_by_id(game_id);
			game_event_t event;
			event.type = GAME_EVENT_BACK;
			game_new_event(game, &event, &game_rsp);
			const char* json = json_helper_simple_str("result",
					game_rsp.ret_str);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/finish-game")) {
			int game_id = get_game_id(hm->query);
			game_t* game = game_manager_get_by_id(game_id);
			game_event_t event;
			event.type = GAME_EVENT_FINISH_GAME;
			game_new_event(game, &event, &game_rsp);
			game_manager_finish(game);
			const char* json = json_helper_simple_str("result",
					game_rsp.ret_str);
			mg_http_reply(c, game_rsp.ret_code, "", json);
			free((char*)json);
		} else {
			struct mg_http_serve_opts opts = {.root_dir = web_root};
			mg_http_serve_dir(c, ev_data, &opts);
		}
	} else if (ev == MG_EV_WS_MSG) {
		// Got websocket frame. Received data is wm->data. Echo it back!
		struct mg_ws_message* wm = (struct mg_ws_message*)ev_data;
		mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
		LOG_DEBUG("Websocket message received: %s", wm->data.ptr);
		if (strcmp(wm->data.ptr, "list") == 0) {
			for (int i = 0; i < MAX_CONNECTIONS; i++) {
				if (connections[i].c != NULL) {
					LOG_INFO("[%d] Conn %d, game_id=%d", i,
							connections[i].c->id, connections[i].game_id);
				}
			}
			return;
		}

		char msg[200];
		// TODO: make it dinamic
		int game_id = get_ws_info(wm->data, msg);
		game_t* game = game_manager_get_by_id(game_id);
		assert(game);
		LOG_TRACE("Game ID: %d, msg: %s", game_id, msg);
		if (strcmp(msg, "status") == 0) {
			game_event_t event;
			event.type = GAME_EVENT_STATUS;
			game_new_event(game, &event, &game_rsp);
		}
	} else if (ev == MG_EV_CLOSE) {
		del_connection(c);
	}
	(void)fn_data;
}

/* Function definitions *******************************************************/

static void new_connection(struct mg_connection* c)
{
	LOG_INFO("New conn id: %ld", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i].c == NULL) {
			connections[i].c = c;
			LOG_INFO("New conection (i=%d, id=%ld)", i, c->id);
			return;
		}
	}
	LOG_WARN("Max number of connections reached");
}

static void del_connection(struct mg_connection* c)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i].c && connections[i].c->id == c->id) {
			connections[i].c = NULL;
			LOG_INFO("Conection closed (i=%d, id=%ld)", i, c->id);
			return;
		}
	}
	// TODO: por qué se cierran las conexiones
	//LOG_WARN("Connection %ld not found", c->id);
}

static int get_game_id(struct mg_str query)
{
	char game_id_str[10];
	struct mg_str param = mg_http_var(query, mg_str("id"));
	strncpy(game_id_str, param.ptr, param.len);
	game_id_str[param.len] = '\0';
	int game_id = atoi(game_id_str);
	LOG_DEBUG("Game ID from params: %d", game_id);
	return game_id;
}

static int get_ws_info(struct mg_str data, char* msg)
{
	char* pt = strtok((char*)data.ptr, ";");
	int game_id = atoi(pt);
	pt = strtok(NULL, ";");
	if (pt == NULL) {
		LOG_ERROR("No message");
		return game_id;
	}
	strncpy(msg, pt, data.len);
	msg[data.len] = '\0';

	return game_id;
}

static void set_game_id(unsigned long c_id, int game_id)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i].c != NULL) {
			LOG_TRACE("Cid=%ld", connections[i].c->id);
			if (connections[i].c->id == c_id) {
				connections[i].game_id = game_id;
				LOG_TRACE("[i=%d] Set conn %ld to game_id=%d", i, c_id, game_id);
				return;
			}
		}
	}
}

/* Public functions ***********************************************************/

void api_init(void) {
	// mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr);
	char url[50];
	snprintf(url, sizeof(url), "ws://%s:%d", API_IP, API_PORT);
	mg_http_listen(&mgr, url, my_handler, NULL);
	LOG_INFO("API started!");
}

void api_fire(void)
{
	mg_mgr_poll(&mgr, 1000);
}

void api_ws_write(const char* msg, int game_id)
{
	(void) game_id;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		// if (connections[i].c != NULL) {
		// 	LOG_TRACE("WS send (i=%d, id=%ld, game_id=%d)", i,
		// 			connections[i].c->id, connections[i].game_id);
		// 	mg_ws_send(connections[i].c, msg, strlen(msg), WEBSOCKET_OP_TEXT);
		// }
		if (connections[i].c != NULL && connections[i].game_id == game_id) {
			mg_ws_send(connections[i].c, msg, strlen(msg), WEBSOCKET_OP_TEXT);
		}
	}
}
