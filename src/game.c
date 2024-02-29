#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "cricket.h"
#include "api.h"

/* Global variables ***********************************************************/

static game_event_t event;
static cricket_t cricket;
static cricket_player_t players[MAX_PLAYERS];

static bool new_event_flag = false;

/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void game_init()
{
	int i = 0;
	for (i = 0; i < MAX_PLAYERS; i++) {
		char* name = malloc(100);
		printf("Player %d name (enter to start): ", i+1);
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';
		if (strlen(name) == 0) {
			free(name);
			if (i == 0) {
				printf("Minimum players: 1\n");
				i--;
				continue;
			}
			break;
		}
		players[i].name = name;
	}

	int max_points = 200;
	int max_rounds = 3;
	cricket_new_game(&cricket, players, i, max_points, max_rounds);
	printf("[Cricket] New game: %d players, %d rounds, max %d points\n", i,
			max_rounds, max_points);
	for (int j = 0; j < i; j++) {
		printf("    %s will play in pos %d\n", cricket.players[j].name, j+1);
	}

	printf("[Cricket] %s's turn\n", cricket.players[0].name);
}

void game_new_event(game_event_t* l_event)
{
	if (new_event_flag) {
		printf("ERROR: Previous event not processed!\n");
	}
	memcpy(&event, l_event, sizeof(event));
	new_event_flag = true;
}

void game_fire()
{
	if (!new_event_flag) {
		return;
	}
	new_event_flag = false;

	dart_shot_t val;

	switch(event.type) {
	case GAME_EVENT_NEW_DART:
		val.number = event.dart.num;
		if (event.dart.zone == ZONE_TRIPLE) {
			val.zone = 3;
		} else if (event.dart.zone == ZONE_DOUBLE) {
			val.zone = 2;
		} else {
			val.zone = 1;
		}
		cricket_new_dart(&cricket, &val);
		char buff[200];
		cricket_status(&cricket, buff);
		api_ws_write(buff);
		cricket_process(&cricket);
		break;
	default:
		break;
	}

}