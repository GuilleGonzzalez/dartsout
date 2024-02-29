#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cricket.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/* Function prototypes ********************************************************/

static bool player_all_closed(cricket_player_t* player);
static cricket_player_t* get_max_score(cricket_t* self);
static bool sector_enabled(cricket_t* self, int sector_number);
static const char* zone_to_str(int zone);
static bool valid_shot(dart_shot_t* ds);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

static bool player_all_closed(cricket_player_t* player)
{
	for (int i = 0; i < N_SECTORS; i++) {
		if (sector_values[i] < 15) {
			continue;
		}
		if (player->shots[i] < 3) {
			return false;
		}
	}
	return true;
}

static cricket_player_t* get_max_score(cricket_t* self)
{
	cricket_player_t* best_player = &self->players[0];
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].game_score >= best_player->game_score) {
			best_player = &self->players[i];
		}
	}
	return best_player;
}

static bool sector_enabled(cricket_t* self, int sector_number)
{
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].shots[sector_number] < 3) {
			return true;
		}
	}
	return false;
}

static const char* zone_to_str(int zone)
{
	const char* str_zone;
	if (zone == 0) {
		str_zone = "triple";
	} else if (zone == 1) {
		str_zone = "double";
	} else {
		str_zone = "single";
	}
	return str_zone;
}

static bool valid_shot(dart_shot_t* ds)
{
	return ds->number > 0 && ds->number <= 20 && ds->zone > 0 && ds->zone <= 4;
}

/* Public functions ***********************************************************/

void cricket_new_game(cricket_t* self, cricket_player_t* players, int n_players,
		int max_score, int max_rounds)
{
	self->n_players = n_players;
	self->round = 1;
	self->max_rounds = max_rounds;
	self->max_score = max_score;
	self->current_player = 0;
	self->darts = 0;
	self->players = players;
	for (int i = 0; i < n_players; i++) {
		self->players[i].game_score = 0;
		self->players[i].round_score = 0;
		memset(self->players[i].shots, 0, N_SECTORS * sizeof(int));
	}
	for (int i = 0; i < N_SECTORS; i++) {
		self->sectors[i].shots = 0;
		self->sectors[i].enabled = 0;
		if (sector_values[i] >= 15) {
			self->sectors[i].enabled = 1;
		}
	}
}

cricket_player_t* cricket_finish_game(cricket_t* self)
{
	cricket_player_t* best_player = get_max_score(self);
	if (self->round > self->max_rounds) {
		return best_player;
	}
	if (player_all_closed(best_player)) {
		return best_player;
	}
	return NULL;
}

void cricket_next_player(cricket_t* self)
{
	self->current_player++;
	if (self->current_player == self->n_players) {
		self->round++;
		if (self->round > self->max_rounds) {
			cricket_finish_game(self);
		}
		self->current_player = 0;
	}
	self->darts = 0;
	self->players[self->current_player].round_score = 0;
}

void cricket_new_dart(cricket_t* self, dart_shot_t* val)
{
	if (!valid_shot(val)) {
		printf("ERROR: Invalid shot!\n");
		return;
	}
	self->darts++;

	cricket_player_t* player = &self->players[self->current_player];
	printf("%s hit %s %d \n", player->name, zone_to_str(val->zone),
			val->number);

	for (int i = 0; i < val->zone; i++) {
		if (!self->sectors[val->number].enabled) {
			return;
		}
		if (player->shots[val->number] < 3) {
			player->shots[val->number]++;
			if (player->shots[val->number] == 3) {
				self->sectors[val->number].enabled =
						sector_enabled(self, val->number);
				printf("%s closed %d\n", player->name, val->number);
			}
		} else {
			player->game_score += sector_values[val->number];
			player->round_score += sector_values[val->number];
		}
	}
}

void cricket_status(cricket_t* self, char* buff)
{
	cricket_player_t* player = &self->players[self->current_player];
	sprintf(buff, "[Cricket] Round: %d/%d, player: %s, score: %d, "
			"round score: %d, darts: %d\n\n",
			self->round, self->max_rounds, player->name,
			player->game_score, player->round_score, self->darts);
}

void cricket_process(cricket_t* self)
{
	cricket_player_t* winner_player = cricket_finish_game(self);
	if (winner_player != NULL) {
		printf("Winner is %s with %d points\n", winner_player->name,
				winner_player->game_score);
		return;
	}
	if (self->darts == MAX_DARTS) {
		cricket_next_player(self);
		printf("[Cricket] %s's turn\n",
				self->players[self->current_player].name);
	}
}