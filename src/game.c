#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "api.h"
#include "cricket.h"
#include "x01.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

static game_t game;
static cricket_t cricket;
static x01_t x01;
static cricket_player_t players[MAX_PLAYERS];
static x01_player_t x01_players[MAX_PLAYERS];

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_init()
{
	game.running = false;
	printf("Game module init!\n");
}

void game_new_event(game_event_t* event, game_event_rsp_t* rsp)
{
	dartboard_shot_t val;
	const char* json;

	// TODO: this is only for cricket!
	switch (event->type) {
	case GAME_EVENT_STATUS:
		if (!game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		json = game_status();
		api_ws_write(json);
		if (game.game == GAME_CRICKET) {
			json = cricket_status(&cricket);
			api_ws_write(json);
			free((char*)json);
		} else if (game.game == GAME_X01) {
			json = x01_status(&x01);
			api_ws_write(json);
			free((char*)json);
		} else {
			printf("ERROR: game not implemented!\n");
		}
		break;
	case GAME_EVENT_NEW_GAME:
		if (game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "A game is running";
			return;
		}
		if (game.n_players == 0) {
			rsp->ret_code = 400;
			rsp->ret_str = "No players added";
			return;
		}
		game.running = true;
		game.game = event->game_id;
		json = game_status();
		api_ws_write(json);
		free((char*)json);
		if (game.game == GAME_CRICKET) {
			int max_points = 200;
			int max_rounds = 25;
			cricket_new_game(&cricket, players, game.n_players, max_points,
					max_rounds);
		} else if (game.game == GAME_X01) {
			int score = 301;
			int max_rounds = 25;
			x01_new_game(&x01, x01_players, game.n_players, score, max_rounds);
		} else {
			printf("ERROR: game not implemented!\n");
		}
		break;
	case GAME_EVENT_NEW_PLAYER:
		if (game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "A game is running";
			return;
		}
		if (game.n_players == MAX_PLAYERS) {
			rsp->ret_code = 400;
			rsp->ret_str = "Max players reached";
			return;
		}
		char* name = malloc(strlen(event->player->name));
		if (name == NULL) {
			rsp->ret_code = 400;
			rsp->ret_str = "Error adding player";
			return;
		}
		strcpy(name, event->player->name);
		game.players[game.n_players].name = name;
		players[game.n_players].p.name = name;
		x01_players[game.n_players].name = name; // TODO: fixit
		game.n_players++;
		json = game_status();
		api_ws_write(json);
		free((char*)json);
		break;
	case GAME_EVENT_NEXT_PLAYER:
		if (!game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		if (game.game == GAME_CRICKET) {
			cricket_next_player(&cricket);
			json = cricket_status(&cricket);
			api_ws_write(json);
			free((char*)json);
		} else if (game.game == GAME_X01) {
			x01_next_player(&x01);
			json = x01_status(&x01);
			api_ws_write(json);
			free((char*)json);
		} else {
			printf("ERROR: game not implemented!\n");
		}
		break;
	case GAME_EVENT_NEW_DART:
		if (!game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		val.number = event->dart.number;
		val.zone = event->dart.zone;
		if (game.game == GAME_CRICKET) {
			bool scoreable = cricket_new_dart(&cricket, &val);
			json = cricket_status(&cricket);
			api_ws_write(json);
			free((char*)json);
			json = json_helper_last_dart(scoreable, val.number, val.zone);
			api_ws_write(json);
			free((char*)json);
			cricket_player_t* winner_player = cricket_check_finish(&cricket);
			if (winner_player != NULL) {
				game.running = false;
				json = game_status();
				api_ws_write(json);
				json = json_helper_winner(winner_player->p.name);
				api_ws_write(json);
				free((char*)json);
				printf("Winner is %s with %d points\n", winner_player->p.name,
						winner_player->game_score);
			}
		} else if (game.game == GAME_X01) {
			x01_new_dart(&x01, &val);
			json = x01_status(&x01);
			api_ws_write(json);
			free((char*)json);
			// TODO game.running = false
		} else {
			printf("ERROR: game not implemented!\n");
		}
		break;
	case GAME_EVENT_FINISH_GAME:
		//TODO: if a player has userid, score must be saved in DB
		if (!game.running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		game.running = false;
		break;
	default:
		break;
	}

	rsp->ret_code = 200;
	rsp->ret_str = "Success";
}

const char* game_status()
{
	return json_helper_game_status(&game);
}
