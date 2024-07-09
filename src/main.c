#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "api.h"
#include "db.h"
#include "game.h"

/* Global variables ***********************************************************/
/* Main function **************************************************************/

int main()
{
	printf("Starting Dartsout!\n");

	api_init();
	game_init();

	for (;;) {
		// In for loop, only api_fire
		api_fire();
	}

	return 0;
}