#include <stdio.h>
#include <unistd.h>
#include "game_manager.h"
#include "api.h"
#include "log.h"

/* Global variables ***********************************************************/
/* Main function **************************************************************/

int main(void)
{
	LOG_INFO("Starting Dartsout!");

	game_manager_init();
	api_init();

	for (;;) {
		// In for loop, only api_fire
		api_fire();
	}

	return 0;
}
