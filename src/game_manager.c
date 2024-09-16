#include <stdio.h>

#include "game_manager.h"
#include "game.h"

#define MAX_GAMES 10

/* Global variables ***********************************************************/

static game_t games[MAX_GAMES];
static int games_running = 0;

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_manager_init(void) {
	for (int i = 0; i < MAX_GAMES; i++) {
		games[i].id = i;
		game_init(&games[i]);
	}
	games_running = 0;

	printf("Game manager started!\n");
}

game_t* game_manager_new(void) {
	for (int i = 0; i < MAX_GAMES; i++) {
		if (games[i].status == GAME_STATUS_DISABLED) {
			games[i].status = GAME_STATUS_PLAYING;
			// games[i].running = true; // en el futuro habría que quitar esto
			games_running++;
			printf("New game with ID=%d\n", games[i].id);
			return &games[i];
		}
	}
	return NULL;
}

game_t* game_manager_get_by_id(int id) {
	for (int i = 0; i < MAX_GAMES; i++) {
		if (games[i].id == id) {
			return &games[i];
		}
	}
	return NULL;
}

void game_manager_finish(game_t* game) {
	game->status = GAME_STATUS_DISABLED;
	// game_reset(game);
	games_running--;
}
