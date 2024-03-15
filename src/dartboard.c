#include <stdlib.h>

#include "dartboard.h"
#include "db.h"
#include "api.h"
#include "player.h"
#include "json_helper.h"

#include <stdio.h>

/* Global variables ***********************************************************/
/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void dartboard_init()
{
}

void dartboard_new_event(dartboard_event_t* event, dartboard_event_rsp_t* rsp)
{
	const char* json;
	int err = 0;
	player_t p;

	switch (event->type) {
	case DARTBOARD_EVENT_REGISTER_PLAYER:
		err = db_players_new(event->player);
		if (err != 0) {
			rsp->ret_code = 400;
			rsp->ret_str = "UserID alredy exists";
			return;
		}
		break;
	case DARTBOARD_EVENT_GET_PLAYER:
		db_players_get(&p, event->userid);
		json = json_helper_reg_player(&p);
		api_ws_write(json);
		free((char*)json);
		break;
	default:
		break;
	}

	rsp->ret_code = 200;
	rsp->ret_str = "Success";
}
