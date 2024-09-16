#include <stdio.h>
#include <unistd.h>
#include "game_manager.h"
#include "api.h"

/* Global variables ***********************************************************/
/* Main function **************************************************************/

int main(void)
{
	printf("Starting Dartsout!\n");

	game_manager_init();
	api_init();

	for (;;) {
		// In for loop, only api_fire
		api_fire();
	}

	return 0;
}
