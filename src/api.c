#include "mongoose.h"
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "cricket.h"
#include "json_helper.h"
#include "game.h"

#define MAX_CONNECTIONS 10

/* Global variables ***********************************************************/

static struct mg_mgr mgr;
static struct mg_connection* connections[MAX_CONNECTIONS];
static const char *s_web_root = "www";

/* Function prototypes ********************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

static void new_connection(struct mg_connection* c);
static void del_connection(struct mg_connection* c);

/* Callbacks ******************************************************************/

//TODO check errors of json functions

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message* hm = (struct mg_http_message*) ev_data;
		if (mg_http_match_uri(hm, "/websocket")) {
			// Upgrade to websocket. From now on, a connection is a full-duplex
			// Websocket connection, which will receive MG_EV_WS_MSG events.
			mg_ws_upgrade(c, hm, NULL);
			new_connection(c);
		} else if (mg_http_match_uri(hm, "/status")) {
			int status = 10;
			const char* json = json_helper_simple_int("status", status);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/new-board")) {
			int next_board_id = 1;
			printf("New board request: asigning board_id=%d\n", next_board_id);
			const char* json = json_helper_simple_int("board_id", next_board_id);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
		} else if (mg_http_match_uri(hm, "/new-dart")) {
			int board_id, num, zone;
			json_helper_new_dart(hm->body.ptr, &board_id, &num, &zone);
			game_event_t event;
			event.type = GAME_EVENT_NEW_DART;
			event.dart.number = num;
			event.dart.zone = zone;
			char* ret_str = game_new_event(&event);
			const char* json = json_helper_simple_str("result", ret_str);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
			printf("New dart: board_id=%d, num=%d, zone=%d\n", board_id, num,
					zone);
		} else if (mg_http_match_uri(hm, "/new-player")) {
			char player_name[20];
			json_helper_new_player(hm->body.ptr, player_name,
					sizeof(player_name));
			game_event_t event;
			event.type = GAME_EVENT_NEW_PLAYER;
			event.player_name = player_name; //TODO: player.name; player.id
			char* ret_str = game_new_event(&event);
			const char* json = json_helper_simple_str("result", ret_str);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
			printf("New player: %s\n", player_name);
		} else if (mg_http_match_uri(hm, "/next-player")) {
			game_event_t event;
			event.type = GAME_EVENT_NEXT_PLAYER;
			char* ret_str = game_new_event(&event);
			const char* json = json_helper_simple_str("result", ret_str);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
			printf("Next player\n");
		} else if (mg_http_match_uri(hm, "/new-game")) {
			int game_id;
			json_helper_new_game(hm->body.ptr, &game_id);
			game_event_t event;
			event.type = GAME_EVENT_NEW_GAME;
			event.game_id = game_id; //TODO: player.name; player.id
			char* ret_str = game_new_event(&event);
			const char* json = json_helper_simple_str("result", ret_str);
			mg_http_reply(c, 200, "", json);
			free((char*)json);
			printf("New game: ID=%d\n", game_id);
		} else {

			struct mg_http_serve_opts opts = {.root_dir = s_web_root};
			mg_http_serve_dir(c, ev_data, &opts);
		}
	} else if (ev == MG_EV_WS_MSG) {
		// Got websocket frame. Received data is wm->data. Echo it back!
		struct mg_ws_message* wm = (struct mg_ws_message*) ev_data;
		mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
		printf("Websocket message received\n");
	} else if (ev == MG_EV_CLOSE) {
		del_connection(c);
	}
	(void) fn_data;
}

/* Function definitions *******************************************************/

static void new_connection(struct mg_connection* c)
{
	printf("New conection with ID=%ld\n", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] == NULL) {
			connections[i] = c;
			return;
		}
	}
	printf("WARNING: Max number of connections reached\n");
}

static void del_connection(struct mg_connection* c)
{
	printf("Closing connection %ld...\n", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] == c) {
			connections[i] = NULL;
			return;
		}
	}
	printf("WARNING: Connection %ld not found\n", c->id);
}

/* Public functions ***********************************************************/

void api_init() {
	// mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr);
	char url[50];
	snprintf(url, sizeof(url), "ws://%s:%d", API_IP, API_PORT);
	mg_http_listen(&mgr, url, my_handler, NULL);
	printf("API started!\n");
}

void api_fire()
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