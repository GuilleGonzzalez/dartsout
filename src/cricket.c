#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cricket.h"
#include "mealy.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/* Function prototypes ********************************************************/

static bool player_all_closed(cricket_player_t* player);
static cricket_player_t* get_max_score(cricket_t* self);
static bool sector_enabled(cricket_t* self, int sector_number);
const char* mult_to_str(int mult);

/* Transition Table ***********************************************************/
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

const char* mult_to_str(int mult)
{
	const char* str_mult;
	if (mult == 1) {
		str_mult = "single";
	} else if (mult == 2) {
		str_mult = "double";
	} else if (mult == 3) {
		str_mult = "triple";
	} else {
		str_mult = "";
	}

	return str_mult;
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
		self->players[i].pos = i;
		self->players[i].game_score = 0;
		memset(self->players[i].shots, 0, N_SECTORS * sizeof(int));
		printf("%s will play in pos %d\n", self->players[i].name,
				self->players[i].pos);
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
}

void cricket_new_dart(cricket_t* self, dart_shot_t* val)
{
	if (val->number < 0 || val->number > 20 ||
			val->multiplier < 0 || val->multiplier > 3) {
		// Impossible
	}

	cricket_player_t* player = &self->players[self->current_player];
	if (!self->sectors[val->number].enabled) {
		return;
	}
	
	printf("%s hit %s %d \n", player->name, mult_to_str(val->multiplier),
			val->number);

	for (int i = 0; i < val->multiplier; i++) {
		if (player->shots[val->number] < 3) {
			player->shots[val->number]++;
			if (player->shots[val->number] == 3) {
				self->sectors[val->number].enabled =
						sector_enabled(self, val->number);
				printf("%s closed %d\n", player->name, val->number);
			}
		} else {
			player->game_score += sector_values[val->number];
		}
	}

}