#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "api.h"
#include "cricket.h"
#include "log.h"
#include "x01.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

// static game_t game;
// static cricket_t cricket;
// static x01_t x01;
// static cricket_player_t players[MAX_PLAYERS];
// static x01_player_t x01_players[MAX_PLAYERS];
// TODO: player_t debería tener dentro una unión con las cosas de los jugadores de cricket, x01, etc.
// NO puede haber variables globales, cricket debe estar dentro de game, también como unión (?)

/* Function prototypes ********************************************************/

static void check_cricket_finish(game_t* game);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

static void check_cricket_finish(game_t* game)
{
	const char* json;
	cricket_player_t* winner_player = cricket_check_finish(&game->cricket);
	if (winner_player != NULL) {
		game->running = false;
		json = game_status(game);
		api_ws_write(json);
		json = json_helper_cricket_winner(&game->cricket, winner_player->p.name);
		api_ws_write(json);
		free((char*)json);
		LOG_INFO("Winner is %s with %d points", winner_player->p.name,
				winner_player->game_score);
	}
}

/* Public functions ***********************************************************/

void game_init(game_t* game)
{
	game->running = false;
	game->status = GAME_STATUS_DISABLED;

	LOG_INFO("Game with ID=%d init!", game->id);
}

void game_new_event(game_t* game, game_event_t* event, game_event_rsp_t* rsp)
{
	dartboard_shot_t val;
	const char* json;

	// TODO: this is only for cricket!
	switch (event->type) {
	case GAME_EVENT_STATUS:
		LOG_DEBUG("Status event received!");
		// TODO: mando aquí el game ID o en un json (?). Hay que mandarlo siempre antes de getStatus
		// json = json_helper_send_game_id(game->id);
		// api_ws_write(json);
		// free((char*)json);
		// LOG_TRACE("Game ID writed");
		json = game_status(game);
		api_ws_write(json);
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		if (game->game_ref == GAME_CRICKET) {
			json = cricket_status(&game->cricket);
			api_ws_write(json);
			free((char*)json);
			LOG_DEBUG("Cricket event sent via WS");
		} else if (game->game_ref == GAME_X01) {
			json = x01_status(&game->x01);
			api_ws_write(json);
			free((char*)json);
			LOG_DEBUG("X01 event sent via WS");
		} else {
			LOG_ERROR("Game not implemented!");
		}
		break;
	case GAME_EVENT_NEW_GAME:
		if (game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "A game is running";
			return;
		}
		if (game->n_players == 0) {
			rsp->ret_code = 400;
			rsp->ret_str = "No players added";
			return;
		}
		game->game_ref = event->game_id;
		game->options = event->options;
		game->running = true;
		json = json_helper_send_game_id(game->id);
		api_ws_write(json);
		free((char*)json);
		if (game->game_ref == GAME_CRICKET) {
			int max_points = 200; //TODO: event.max_points
			int max_rounds = 25;  //TODO: event.max_rounds
			cricket_options_t options = event->options;
			LOG_DEBUG("Options: %d", options);
			cricket_new_game(&game->cricket,
					game->cricket_players, game->n_players, options,
					max_points, max_rounds);
		} else if (game->game_ref == GAME_X01) {
			int score = 301;      //TODO: event.score
			int max_rounds = 25;  //TODO: event.max_rounds
			x01_options_t options = event->options;
			LOG_DEBUG("Nplayers: %d", game->n_players);
			LOG_DEBUG("Options: %d", options);
			LOG_DEBUG("Score: %d", score);
			LOG_DEBUG("Max rouds: %d", max_rounds);
			x01_new_game(&game->x01, game->x01_players, game->n_players, options,
					max_rounds);
		} else {
			LOG_ERROR("Game not implemented!");
		}
		break;
	case GAME_EVENT_NEW_PLAYER:
		if (game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "A game is running";
			return;
		}
		if (game->n_players == MAX_PLAYERS) {
			rsp->ret_code = 400;
			rsp->ret_str = "Max players reached";
			return;
		}
		char* name = malloc(strlen(event->player.name));
		if (name == NULL) {
			rsp->ret_code = 400;
			rsp->ret_str = "Error adding player";
			return;
		}
		strcpy(name, event->player.name);
		game->cricket_players[game->n_players].p.name = name; // Only cricket player==
		// players[game->n_players].p.name = name;
		// x01_players[game->n_players].p.name = name; // TODO: fixit
		game->n_players++;
		json = game_status(game);
		api_ws_write(json);
		free((char*)json);
		break;
	case GAME_EVENT_NEXT_PLAYER:
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		if (game->game_ref == GAME_CRICKET) {
			cricket_next_player(&game->cricket);
			json = cricket_status(&game->cricket);
			api_ws_write(json);
			free((char*)json);
			check_cricket_finish(game);
		} else if (game->game_ref == GAME_X01) {
			x01_next_player(&game->x01);
			json = x01_status(&game->x01);
			api_ws_write(json);
			free((char*)json);
		} else {
			LOG_ERROR("Game not implemented!");
		}
		break;
	case GAME_EVENT_NEW_DART:
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		val.number = event->dart.number;
		val.zone = event->dart.zone;
		if (game->game_ref == GAME_CRICKET) {
			bool scoreable = cricket_new_dart(&game->cricket, &val);
			json = cricket_status(&game->cricket);
			api_ws_write(json);
			free((char*)json);
			json = json_helper_last_dart(scoreable, val.number, val.zone);
			api_ws_write(json);
			free((char*)json);
			check_cricket_finish(game);
		} else if (game->game_ref == GAME_X01) {
			bool valid = x01_new_dart(&game->x01, &val);
			json = x01_status(&game->x01);
			api_ws_write(json);
			free((char*)json);
			json = json_helper_last_dart(valid, val.number, val.zone);
			api_ws_write(json);
			free((char*)json);
			// TODO: issue: puede ser que el juego se acabe al pasar de ronda
			x01_player_t* winner_player = x01_check_finish(&game->x01);
			if (winner_player != NULL) {
				game->running = false;
				json = game_status(game);
				api_ws_write(json);
				json = json_helper_winner(winner_player->p.name);
				api_ws_write(json);
				free((char*)json);
				LOG_INFO("Winner is %s with %d points", winner_player->p.name,
						winner_player->game_score);
			}
		} else {
			LOG_ERROR("Game not implemented!");
		}
		break;
	case GAME_EVENT_FINISH_GAME:
		game_reset(game);
		LOG_INFO("Game finished!");
		break;
	default:
		break;
	}
	rsp->ret_code = 200;
	rsp->ret_str = "Success";
}

const char* game_status(game_t* game)
{
	return json_helper_game_status(game);
}

void game_reset(game_t* game)
{
	game->status = GAME_STATUS_DISABLED;
	game->running = false;
	game->options = 0;
	game->n_players = 0;
	LOG_INFO("Game with ID=%d reset!", game->id);
}
