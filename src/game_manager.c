#include <stdio.h>

#include "game_manager.h"
#include "game.h"
#include "cricket.h"
#include "x01.h"
#include "log.h"

#define MAX_GAMES 10

/* Global variables ***********************************************************/

static game_t* games[MAX_GAMES];

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_manager_init(void)
{
	for (int i = 0; i < MAX_GAMES; i++) {
		games[i] = NULL;
	}

	LOG_INFO("Game manager started!");
}

game_t* game_manager_new(game_ref_t game_ref, int options)
{
	int game_id = 0;
	for (int i = 0; i < MAX_GAMES; i++) {
		if (games[i] == NULL) {
			game_id = i;
			LOG_INFO("New game with ID=%d", game_id);
			break;
		}
	}

	if (game_ref == GAME_CRICKET) {
		int max_points = 200; //TODO: event.max_points
		int max_rounds = 25;  //TODO: event.max_rounds
		games[game_id] = (game_t*)cricket_new_game(game_id, options, max_points,
				max_rounds);
		return games[game_id];
	} else if (game_ref == GAME_X01) {
		int max_rounds = 25;  //TODO: event.max_rounds
		games[game_id] = (game_t*)x01_new_game(game_id, options, max_rounds);
		return games[game_id];
	} else {
		LOG_ERROR("Game not implemented!");
	}

	return NULL;
}

game_t* game_manager_get_by_id(int id)
{
	if (games[id] != NULL) {
		return games[id];
	}
	return NULL;
}

void game_manager_finish(game_t* game)
{
	game_delete(game);
}
