#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "cricket.h"
#include "api.h"

/* Global variables ***********************************************************/

static cricket_t cricket;
static cricket_player_t players[MAX_PLAYERS];
static int n_players = 0;

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_init()
{
}

char* game_new_event(game_event_t* event)
{
	dart_shot_t val;

	switch(event->type) {
	case GAME_EVENT_NEW_GAME:
		if (n_players <= 0) {
			return "No players added";
		}
		int max_points = 200;
		int max_rounds = 3;
		cricket_new_game(&cricket, players, n_players, max_points, max_rounds);
		printf("[Cricket] New game: %d players, %d rounds, max %d points\n",
				n_players, max_rounds, max_points);
		for (int j = 0; j < n_players; j++) {
			printf("    %s will play in pos %d\n", cricket.players[j].name, j+1);
		}
		printf("[Cricket] %s's turn\n", cricket.players[0].name);
		break;
	case GAME_EVENT_NEW_PLAYER:
		if (n_players == MAX_PLAYERS) {
			return "Max players reached";
		}
		char* name = malloc(strlen(event->player_name));
		if (name == NULL) {
			return "Error adding player";
		}
		strcpy(name, event->player_name);
		players[n_players].name = name;
		n_players++;
		break;
	case GAME_EVENT_NEW_DART:
		val.number = event->dart.num;
		if (event->dart.zone == ZONE_TRIPLE) {
			val.zone = 3;
		} else if (event->dart.zone == ZONE_DOUBLE) {
			val.zone = 2;
		} else {
			val.zone = 1;
		}
		cricket_new_dart(&cricket, &val);
		const char* json = cricket_status(&cricket);
		api_ws_write(json);
		free((char*)json);
		cricket_process(&cricket);
		break;
	default:
		break;
	}

	return "Success";
}