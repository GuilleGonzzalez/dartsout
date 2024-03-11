#include "dartboard.h"
#include "db.h"

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
	// const char* json;
	int err = 0;

	switch (event->type) {
	case DARTBOARD_EVENT_REGISTER_PLAYER:
		err = db_players_new(event->player);
		break;
		if (err != 0) {
			rsp->ret_code = 400;
			rsp->ret_str = "UserID alredy exists";
			return;
		}
	default:
		break;
	}

	rsp->ret_code = 200;
	rsp->ret_str = "Success";
}
