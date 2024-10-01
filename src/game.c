#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "api.h"
#include "array.h"
#include "log.h"
#include "player.h"
#include "json_helper.h"

/* Global variables ***********************************************************/
/* Function prototypes ********************************************************/

static void check_winner(game_t* game);
static void save_game(game_t* game);
static void restore_game(game_t* game);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

static void check_winner(game_t* game)
{
	player_t* winner_player;
	const char* winner_json = game->cbs->check_finish_cb(game, &winner_player);
	if (winner_json != NULL) {
		game->running = false;
		const char* json = json_helper_game_status(game);
		api_ws_write(json, game->id);
		api_ws_write(winner_json, game->id);
		free((char*)winner_json);
		free((char*)json);
		LOG_INFO("Winner is %s", winner_player->name);
	}
}

static void save_game(game_t* game)
{
	void* game_state = game->cbs->save_state_cb(game);
	assert(game_state);
	array_append(game->game_states, game_state);
}

static void restore_game(game_t* game)
{
	// TODO: if first state, no back
	void* game_state = array_pop(game->game_states);
	assert(game_state);
	bool success = game->cbs->restore_state_cb(game, game_state);
	if (!success) {
		LOG_ERROR("Game could not be restored");
	}
}

/* Public functions ***********************************************************/

void game_init(game_t* game, int id, game_cbs_t* cbs)
{
	game->id = id;
	game->running = false;
	game->n_players = 0;
	game->players = NULL;
	game->cbs = cbs;
	// TODO: check cbs not null
	game->game_states = array_new(0);
}

void game_new_event(game_t* game, game_event_t* event, game_event_rsp_t* rsp)
{
	dartboard_shot_t val;
	const char* json;

	switch (event->type) {
	case GAME_EVENT_STATUS:
		LOG_DEBUG("Status event received!");
		json = json_helper_game_status(game);
		api_ws_write(json, game->id);
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		json = game->cbs->status_cb(game);
		api_ws_write(json, game->id);
		free((char*)json);
		LOG_DEBUG("Game status sent via WS");
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
		game->cbs->start_cb(game);
		json = json_helper_send_game_id(game->id);
		api_ws_write(json, game->id);
		free((char*)json);
		LOG_INFO("Game with ID=%d started!", game->id);
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
		game->players = realloc(game->players,
				(game->n_players + 1) * sizeof(player_t));
		assert(game->players);
		strcpy(name, event->player.name);
		game->players[game->n_players].userid= "";
		game->players[game->n_players].name = name;
		game->n_players++;
		break;
	case GAME_EVENT_NEXT_PLAYER:
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		save_game(game);
		game->cbs->next_player_cb(game);
		json = game->cbs->status_cb(game);
		api_ws_write(json, game->id);
		free((char*)json);
		check_winner(game);
		break;
	case GAME_EVENT_NEW_DART:
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		//TODO: si no se pueden tirar mas dardos, no guardar
		//TODO: sonidito de atrás
		//TODO: free de todos los estados
		save_game(game);
		val.number = event->dart.number;
		val.zone = event->dart.zone;
		bool scoreable = game->cbs->new_dart_cb(game, &val);
		json = game->cbs->status_cb(game);
		api_ws_write(json, game->id);
		free((char*)json);
		json = json_helper_last_dart(scoreable, val.number, val.zone);
		api_ws_write(json, game->id);
		free((char*)json);
		check_winner(game);
		break;
	case GAME_EVENT_BACK:
		if (!game->running) {
			rsp->ret_code = 400;
			rsp->ret_str = "No game running";
			return;
		}
		restore_game(game);
		json = game->cbs->status_cb(game);
		api_ws_write(json, game->id);
		free((char*)json);
		LOG_INFO("Back action performed!");
		break;
	case GAME_EVENT_FINISH_GAME:
		LOG_INFO("Game with ID=%d finished!", game->id);
		break;
	default:
		break;
	}
	rsp->ret_code = 200;
	rsp->ret_str = "Success";
}

void game_finish(game_t* game)
{
	game->running = false;
	game->options = 0;
	game->n_players = 0;
}

void game_delete(game_t* game)
{
	game->cbs->delete_cb(game);

	for (int i = 0; i < game->n_players; i++) {
		free((char*)game->players[i].name);
	}
	free(game->players);

	// TODO: free all states
	state_t* state;
	while ((state = (state_t*) array_pop(game->game_states)) != NULL) {
		state->delete_cb(state);
	}
	array_free(game->game_states);
}
