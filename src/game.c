#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "cricket.h"

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
	memcpy(&event, l_event, sizeof(event));
	new_event_flag = true;
}

void game_fire()
{
	if (!new_event_flag) {
		return;
	}
	new_event_flag = false;

	switch(event.type) {
	case GAME_EVENT_NEW_DART:
	{
		dart_shot_t val;
		val.number = event.dart.num;
		if (event.dart.zone == ZONE_TRIPLE) {
			val.multiplier = 3;
		} else if (event.dart.zone == ZONE_DOUBLE) {
			val.multiplier = 2;
		} else {
			val.multiplier = 1;
		}
		cricket_new_dart(&cricket, &val);
		cricket.darts++;
		cricket_player_t* winner_player = cricket_finish_game(&cricket);
		if (winner_player != NULL) {
			cricket_finish_game(&cricket);
			printf("Winner is %s with %d points\n", winner_player->name,
					winner_player->game_score);
			return;
		}
		cricket_player_t* player = &cricket.players[cricket.current_player];
		printf("[Cricket] Round: %d/%d, player: %s, score: %d, round score: %d, darts: %d\n\n",
				cricket.round, cricket.max_rounds, player->name,
				player->game_score, player->round_score, cricket.darts);
		if (cricket.darts == MAX_DARTS) {
			cricket_next_player(&cricket);
			printf("[Cricket] %s's turn\n",
					cricket.players[cricket.current_player].name);
		}
		break;
	}
	default:
		break;
	}

}