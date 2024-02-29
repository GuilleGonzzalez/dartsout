#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "api.h"
#include "db.h"
#include "game.h"
#include "websocket.h"

/* Global variables ***********************************************************/
/* Main function **************************************************************/

int main()
{
	printf("Starting Dartsout!\n");

	// websockets_init();
	api_init();
	game_init();

	// db_init();
	// db_players_new(2, "Alex"); //TODO: pasarle un tipo player_t
	// db_players_get();
	// db_cricket_new(1, 3.4);
	// db_cricket_get();

	for (;;) {
		// In for loop, only api_fire
		api_fire();
		game_fire();
	}

	return 0;
}