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
	dartboard_shot_t val;
	const char* json;

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
		json = cricket_status(&cricket);
		api_ws_write(json);
		free((char*)json);
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
	case GAME_EVENT_NEXT_PLAYER:
		cricket_next_player(&cricket);
		json = cricket_status(&cricket);
		api_ws_write(json);
		free((char*)json);
		break;
	case GAME_EVENT_NEW_DART:
		val.number = event->dart.number;
		val.zone = event->dart.zone;
		cricket_new_dart(&cricket, &val);
		json = cricket_status(&cricket);
		api_ws_write(json);
		free((char*)json);
		cricket_player_t* winner_player = cricket_check_finish(&cricket);
		if (winner_player != NULL) {
			game_event_t event;
			event.type = GAME_EVENT_FINISH;
			char* ret_str = game_new_event(&event);
			api_ws_write(ret_str);
			printf("Winner is %s with %d points\n", winner_player->name,
					winner_player->game_score);
		}
		break;
	case GAME_EVENT_FINISH:
		printf("Game finished!\n");
		break;
	default:
		break;
	}

	return "Success";
}