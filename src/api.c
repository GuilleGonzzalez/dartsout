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

/* Function prototypes ********************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

static void new_connection(struct mg_connection* c);
static void del_connection(struct mg_connection* c);

/* Callbacks ******************************************************************/

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
			char* json = json_helper_simple_int("status", status);
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/new-board")) {
			int next_board_id = 1;
			printf("New board request: asigning board_id=%d\n", next_board_id);
			char* json = json_helper_simple_int("board_id", next_board_id);
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/new-dart")) {
			int board_id, num, zone;
			json_helper_new_dart(hm->body.ptr, &board_id, &num, &zone);
			game_event_t event;
			event.type = GAME_EVENT_NEW_DART;
			event.dart.num = num;
			event.dart.zone = zone;
			game_new_event(&event);
			printf("New dart: board_id=%d, num=%d, zone=%d\n", board_id, num,
					zone);
			char* json = json_helper_simple_str("result", "OK");
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/new-game")) {
			// GAME
			// PLAYERS[]
		} else {
			char* json = json_helper_simple_str("error", "Not found");
			mg_http_reply(c, 404, "", json);
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
	// printf("New conection with ID=%ld\n", c->id);
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
	// printf("Closing connection %ld...\n", c->id);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (connections[i] == c) {
			connections[i] = NULL;
			return;
		}
	}
	// printf("WARNING: Connection %ld not found\n", c->id);
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