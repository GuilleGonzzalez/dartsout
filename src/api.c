#include "mongoose.h"
#include <stdio.h>
#include <string.h>

#include "cricket.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

static struct mg_mgr mgr;

/* Function prototypes ********************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

/* Callbacks ******************************************************************/

static void my_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message* hm = (struct mg_http_message*) ev_data;
		if (mg_http_match_uri(hm, "/status")) {
			int status = 10;
			char* json = json_helper_simple_int("status", status);
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/new-board")) {
			int next_board_id = 1;
			printf("New board request: asigning board_id=%d\n", next_board_id);
			char* json = json_helper_simple_int("board_id", next_board_id);
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/new-dart")) {
			int board_id, num, mult;
			json_helper_new_dart(hm->body.ptr, &board_id, &num, &mult);
			// cricket_new_dart((int)num, (int)mult);
			printf("New dart: board_id=%d, num=%d, mult=%d\n", board_id, num, mult);
			if (0) {
				char* json = json_helper_simple_str("result", "Invalid state");
				mg_http_reply(c, 400, "", json);
			} else {
				char* json = json_helper_simple_str("result", "OK");
				mg_http_reply(c, 200, "", json);
			}
		} else {
			char* json = json_helper_simple_str("error", "Not found");
			mg_http_reply(c, 404, "", json);
		}
	}
	(void) fn_data;
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void api_init() {
	mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://localhost:8000", my_handler, NULL);
	printf("API started!\n");
}

void api_fire()
{
	mg_mgr_poll(&mgr, 1000);
}