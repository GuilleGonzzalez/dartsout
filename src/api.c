#include "mongoose.h"
#include <stdio.h>
#include <string.h>

/* Global variables ***********************************************************/

static struct mg_mgr mgr;

/* Function prototypes ********************************************************/

static void mi_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data);

/* Callbacks ******************************************************************/

static void mi_handler(struct mg_connection* c, int ev, void* ev_data,
		void* fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message* hm = (struct mg_http_message*) ev_data;
		if (mg_http_match_uri(hm, "/status")) {
			char json[20];
			int status = 10;
			sprintf(json, "{\"%s\":%d}\n", "status", status);
			mg_http_reply(c, 200, "", json);
		} else if (mg_http_match_uri(hm, "/players")) {
			char json[20];
			int n_players = 2;
			sprintf(json, "{\"%s\":%d}\n", "n_players", n_players);
			mg_http_reply(c, 200, "", json);
		} else {
			mg_http_reply(c, 404, "", "{\"error\":\"Not found\"}\n");	 
		}
	}
	(void) fn_data;
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void api_init() {
	mg_log_set(MG_LL_DEBUG);
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://localhost:8000", mi_handler, NULL);
	printf("API started!\n");
}

void api_fire()
{
	mg_mgr_poll(&mgr, 1000);
}