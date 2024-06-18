#include <stdio.h>
#include <string.h>

#include "x01.h"
#include "dartboard.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/* Function prototypes ********************************************************/

static const char* zone_to_str(dartboard_zone_t zone);
static bool valid_shot(dartboard_shot_t* ds);
static x01_player_t* get_min_score(x01_t* self);
static void fill_darts_null(x01_t* self);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

//TODO: move to dartboard (?)
static const char* zone_to_str(dartboard_zone_t zone)
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

static bool valid_shot(dartboard_shot_t* ds)
{
	// NOTE: 0 is bull
	printf("NUMBER: %d, ZONE: %d\n", ds->number, ds->zone);
	return ds->number >= 0 && ds->number <= 20 && ds->zone >= 0 && ds->zone <= 4;
}

static x01_player_t* get_min_score(x01_t* self)
{
	x01_player_t* best_player = &self->players[0];
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].game_score < best_player->game_score) {
			best_player = &self->players[i];
		}
	}
	return best_player;
}

static void fill_darts_null(x01_t* self)
{
	for (int i = self->darts; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
		self->darts = MAX_DARTS;
	}
}

/* Public functions ***********************************************************/

void x01_new_game(x01_t* self, x01_player_t* players, int n_players,
		x01_options_t options, int score, int max_rounds)
{
	self->score = score;
	self->n_players = n_players;
	self->options = options;
	self->round = 1;
	self->prev_score = 0;
	self->max_rounds = max_rounds;
	self->current_player = 0;
	self->darts = 0;
	self->players = players;
	for (int i = 0; i < n_players; i++) {
		self->players[i].game_score = score;
		self->players[i].round_score = 0;
		memset(self->players[i].shots, 0, N_SECTORS * sizeof(int));
	}
	for (int i = 0; i < N_SECTORS; i++) {
		self->sectors[i].shots = 0;
		self->sectors[i].enabled = 1;
	}
	for (int i = 0; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
	}
	printf("[%d] New game: %d players, %d rounds\n",
				score, self->n_players, self->max_rounds);
}

x01_player_t* x01_check_finish(x01_t* self)
{
	x01_player_t* best_player = get_min_score(self);
	// If last round
	if (self->round == self->max_rounds &&
			self->current_player == self->n_players - 1 &&
			self->darts == MAX_DARTS) {
		return best_player;
	}
	if (best_player->game_score == 0) {
		return best_player;
	}
	return NULL;
}

void x01_next_player(x01_t* self)
{
	self->current_player++;
	if (self->current_player == self->n_players) {
		self->round++;
		if (self->round > self->max_rounds) {
			printf("No more rounds!\n");
		}
		self->current_player = 0;
	}
	self->darts = 0;
	x01_player_t current_player = self->players[self->current_player];
	self->prev_score = current_player.game_score;
	current_player.round_score = 0;
}

bool x01_new_dart(x01_t* self, dartboard_shot_t* val)
{
	bool valid = true;

	if (!valid_shot(val)) {
		printf("ERROR: Invalid shot!\n");
		return false;
	}
	if (self->darts == MAX_DARTS) {
		printf("No more darts!\n");
		return false;
	}
	self->dart_scores[self->darts].number = val->number;
	self->dart_scores[self->darts].zone = val->zone;
	self->darts++;

	x01_player_t* player = &self->players[self->current_player];
	printf("%s hit %s %d \n", player->p.name, zone_to_str(val->zone),
			val->number);
	int mult = 1;
	if (val->zone == ZONE_TRIPLE) {
		mult = 3;
	} else if (val->zone == ZONE_DOUBLE) {
		mult = 2;
	}
	int score = player->game_score;
	score -= (sector_values[val->number] * mult);
	if (player->game_score == self->score) {
		if ((self->options & double_in) && (val->zone != ZONE_DOUBLE)) {
			printf("Not double in\n");
			return false;
		}
	} else if (score < 0) {
		valid = false;
		// TODO: this is a temp solution
		fill_darts_null(self);
		score = self->prev_score;
	} else if ((score == 1) && (self->options &= double_out)) {
		score = 2;
		// TODO: this is a temp solution
		fill_darts_null(self);
	} else if (score == 0) {
		if ((self->options & double_out) && (val->zone != ZONE_DOUBLE)) {
			valid = false;
			printf("Not double out\n");
			score = self->prev_score;
			// TODO: this is a temp solution
			fill_darts_null(self);
		} else {
			printf("%s win! %d\n", player->p.name, val->number);
		}
	}
	player->game_score = score;
	player->round_score += sector_values[val->number];

	return valid;
}

const char* x01_status(x01_t* self)
{
	return json_helper_x01_status(self);
}
