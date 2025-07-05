#include <assert.h>
#include <stdlib.h>

#include "game_manager.h"
#include "game.h"
#include "cricket.h"
#include "x01.h"
#include "log.h"

#define INACTIVITY_LIMIT (2*60*60)   // 2 horas

/* Global variables ***********************************************************/

static game_t** games;
static int next_id = 0;
static int n_games = 0;

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/

void game_manager_timer_cb(void* args)
{
	(void)args; // Unused parameter

	time_t now = time(NULL);

	for (int i = 0; i < n_games; i++) {
		if (games[i]->running && now - games[i]->last_activity > INACTIVITY_LIMIT) {
			//TODO: finish game
			LOG_INFO("Game %d has been inactive for %ld seconds. Finishing it.",
					games[i]->id, now - games[i]->last_activity);
			continue;
		}

		// if (games[i]->running) {
			// int elapsed = (int)(now - games[i]->started_at);
			// if (elapsed / TIMER_PUSH_PERIOD_S != g->last_timer_push) {
			// 	g->last_timer_push = elapsed / TIMER_PUSH_PERIOD_S;
			// 	//PUSH por ws
			// }
		// }
	}
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_manager_init(void)
{
	LOG_INFO("Game manager started!");
}

game_t* game_manager_new(game_ref_t game_ref, int options)
{
	int game_id = next_id;
	games = realloc(games, (n_games + 1) * sizeof(game_t*));
	assert(games);
	game_t* game = NULL;
	if (game_ref == GAME_CRICKET) {
		int max_points = 200; //TODO: event.max_points
		int max_rounds = 25;  //TODO: event.max_rounds
		game = (game_t*)cricket_new_game(game_id, options, max_points,
				max_rounds);
		games[n_games++] = game;
		next_id++;
	} else if (game_ref == GAME_X01) {
		int max_rounds = 25;  //TODO: event.max_rounds
		game = (game_t*)x01_new_game(game_id, options, max_rounds);
		games[n_games++] = game;
		next_id++;
	} else {
		LOG_ERROR("Game not implemented!");
	}

	return game;
}

game_t* game_manager_get_by_id(int id)
{
	for (int i = 0; i < n_games; i++) {
		if (games[i]->id == id) {
			return games[i];
		}
	}
	return NULL;
}

game_t* game_manager_get_by_dartboard(int dartboard_id)
{
	for (int i = 0; i < n_games; i++) {
		if (game_has_dartboard(games[i], dartboard_id)) {
			return games[i];
		}
	}
	return NULL;
}

void game_manager_finish(game_t* game)
{
	LOG_INFO("Finishing game %d", game->id);

	n_games--;
	game_t** games_new = malloc(n_games * sizeof(game_t));

	int i_new = 0;
	for (int i = 0; i < n_games + 1; i++) {
		if (games[i] == game) {
			continue;
		}
		assert(i_new < n_games);
		games_new[i_new++] = games[i];
	}

	free(games);
	games = games_new;

	game_delete(game);
}
