#include "api.h"

#include "game_manager.h"
#include "mongoose.h"
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "json_helper.h"
#include "game.h"
#include "log.h"

#define MAX_CONNECTIONS 10

/* Global variables ***********************************************************/

static struct mg_mgr mgr;
static struct mg_connection* connections[MAX_CONNECTIONS];
static const char* web_root = "www";

/* Function prototypes ********************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

static void new_connection(struct mg_connection* c);
static void del_connection(struct mg_connection* c);

static int get_game_id(struct mg_str query);
static int get_ws_info(struct mg_str data, char* msg);

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
			mg_ws_upgrade(c, hm, NULL);
			new_connection(c);
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
			game_t* game = game_manager_new();
			for (int i = 0; i < n_players; i++) {
				game_event_t event;
				event.type = GAME_EVENT_NEW_PLAYER;
				event.player.name = players[i];
				game_new_event(game, &event, &game_rsp);
			}
			game_event_t event;
			event.type = GAME_EVENT_NEW_GAME;
			event.game_id = game_ref;
			event.options = options;
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
		char msg[20];
		int game_id = get_ws_info(wm->data, msg);
		game_t* game = game_manager_get_by_id(game_id);
		LOG_TRACE("Game ID: %d, msg: %s", game_id, msg);
		if (strcmp(msg, "status") == 0) {
			game_event_t event;
			event.type = GAME_EVENT_STATUS;
			game_new_event(game, &event, &game_rsp);
		}
		LOG_DEBUG("Websocket message received: %s", wm->data.ptr);
	} else if (ev == MG_EV_CLOSE) {
		del_connection(c);
	}
	(void)fn_data;
}

/* Function definitions *******************************************************/

static void new_connection(struct mg_connection* c)
{
	LOG_DEBUG("New conection with ID=%ld", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] == NULL) {
			connections[i] = c;
			return;
		}
	}
	LOG_WARN("Max number of connections reached");
}

static void del_connection(struct mg_connection* c)
{
	LOG_DEBUG("Closing connection %ld...", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] == c) {
			connections[i] = NULL;
			return;
		}
	}
	LOG_WARN("Connection %ld not found", c->id);
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
	char* pt = strtok((char*)data.ptr, ",");
	int game_id = atoi(pt);
	pt = strtok(NULL, ",");
	strncpy(msg, pt, data.len);
	msg[data.len] = '\0';

	return game_id;
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

void api_ws_write(const char* msg)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] != NULL) {
			mg_ws_send(connections[i], msg, strlen(msg), WEBSOCKET_OP_TEXT);
		}
	}
}
