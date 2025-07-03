#include <unistd.h>
#include <signal.h>
#include "game_manager.h"
#include "api.h"
#include "log.h"

/* Global variables ***********************************************************/

static int running = 1;

/* Function prototypes ********************************************************/

static void signal_handler(int sig);

/* Function definitions *******************************************************/

static void signal_handler(int sig) {
	(void)sig;
	running = 0;
}

/* Main function **************************************************************/

int main(void)
{
	LOG_INFO("Starting Dartsout!");

	game_manager_init();
	api_init();

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	for (;;) {
		// In for loop, only api_fire
		api_fire();

		if (!running) {
			LOG_INFO("Shutting down Dartsout!");
			break;
		}
	}

	api_free();

	return 0;
}
