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

	api_init();
	game_init();

	db_init();
	// db_cricket_new(1, 3.24);
	// db_cricket_get();
	player_t p;
	db_players_get(&p, "guille");
	player_t p_new;
	p_new.userid = "guille";
	p_new.name = "Guille";
	db_players_new(&p_new);
	db_players_get(&p, "guille");

	for (;;) {
		// In for loop, only api_fire
		api_fire();
	}

	return 0;
}