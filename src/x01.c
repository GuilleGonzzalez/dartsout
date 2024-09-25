#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "x01.h"
#include "dartboard.h"
#include "json_helper.h"
#include "log.h"
#include "player.h"
#include "utils.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/* Function prototypes ********************************************************/

static void start(game_t* game);
static void next_player(game_t* game);
static bool new_dart(game_t* game, dartboard_shot_t* val);
static const char* check_finish(game_t* game, player_t** winner_player);
static const char* status(game_t* self);
static void delete(game_t* game);

static game_cbs_t cbs = {
	.start_cb = start,
	.next_player_cb = next_player,
	.new_dart_cb = new_dart,
	.check_finish_cb = check_finish,
	.status_cb = status,
	.delete_cb = delete,
};

static int get_score(int options);
static void reset_darts(x01_t* self);
static x01_player_t* get_min_score(x01_t* self);
static void fill_darts_null(x01_t* self);

/* Callbacks ******************************************************************/

static void start(game_t* game)
{
	x01_t* self = (x01_t*)game;
	self->players = malloc(self->game.n_players * sizeof(x01_player_t));
	assert(self->players);
	for (int i = 0; i < self->game.n_players; i++) {
		self->players[i].game_score = self->score;
		self->players[i].round_score = 0;
		memset(self->players[i].shots, 0, N_ENABLED * sizeof(int));
	}

	LOG_INFO("New game: %d players, %d rounds",
			self->game.n_players, self->max_rounds);
}

static void next_player(game_t* game)
{
	x01_t* self = (x01_t*)game;
	reset_darts(self);
	self->current_player++;
	if (self->current_player == self->game.n_players) {
		self->round++;
		if (self->round > self->max_rounds) {
			LOG_WARN("No more rounds!");
		}
		self->current_player = 0;
	}
	self->darts = 0;
	x01_player_t current_player = self->players[self->current_player];
	self->prev_score = current_player.game_score;
	current_player.round_score = 0;
}

static bool new_dart(game_t* game, dartboard_shot_t* val)
{
	x01_t* self = (x01_t*)game;

	bool valid = true;

	if (!utils_valid_shot(val)) {
		return false;
	}
	if (self->darts == MAX_DARTS) {
		LOG_WARN("No more darts!");
		return false;
	}
	self->dart_scores[self->darts].number = val->number;
	self->dart_scores[self->darts].zone = val->zone;
	self->darts++;

	x01_player_t* player = &self->players[self->current_player];
	LOG_INFO("%s hit %s %d", self->game.players[self->current_player].name,
			utils_zone_to_str(val->zone), val->number);
	int mult = utils_mult_from_zone(val->zone);
	int score = player->game_score;
	score -= (sector_values[val->number] * mult);
	if (player->game_score == self->score) {
		if ((self->options & double_in) && (val->zone != ZONE_DOUBLE)) {
			LOG_WARN("Not double in");
			return false;
		}
	} else if (score < 0) {
		valid = false;
		// TODO: this is a temp solution
		fill_darts_null(self);
		score = self->prev_score;
	} else if ((score == 1) && (self->options &= double_out)) {
		valid = false;
		LOG_WARN("Not double out");
		score = self->prev_score;
		// TODO: this is a temp solution
		fill_darts_null(self);
	} else if (score == 0) {
		if ((self->options & double_out) && (val->zone != ZONE_DOUBLE)) {
			valid = false;
			LOG_WARN("Not double out");
			score = self->prev_score;
			// TODO: this is a temp solution
			fill_darts_null(self);
		} else {
			LOG_INFO("%s win!", self->game.players[self->current_player].name);
		}
	}
	player->game_score = score;
	player->round_score += sector_values[val->number];

	return valid;
}

static const char* check_finish(game_t* game, player_t** winner_player)
{
	x01_t* self = (x01_t*)game;

	x01_player_t* best_player = get_min_score(self);
	// If last round
	if ((self->round > self->max_rounds) ||
			(self->round == self->max_rounds &&
			self->darts == MAX_DARTS &&
			self->current_player == self->game.n_players - 1)) {
		*winner_player = x01_get_player(self, best_player);
		return json_helper_winner((*winner_player)->name);
	}
	if (best_player->game_score == 0) {
		*winner_player = x01_get_player(self, best_player);
		return json_helper_winner((*winner_player)->name);
	}
	return NULL;
}

static const char* status(game_t* self)
{
	return json_helper_x01_status((x01_t*)self);
}

static void delete(game_t* game)
{
	x01_t* self = (x01_t*)game;

	free(self->players);
	free(self);
}

/* Function definitions *******************************************************/

static int get_score(int options)
{
	switch(options >> 2) {
	case (1 << 0):
		return 301;
	case (1 << 1):
		return 501;
	case (1 << 2):
		return 701;
	case (1 << 3):
		return 901;
	case (1 << 4):
		return 1001;
	default:
		return 301;
	}
}

static void reset_darts(x01_t* self)
{
	for (int i = 0; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
	}
}

static x01_player_t* get_min_score(x01_t* self)
{
	x01_player_t* best_player = &self->players[0];
	for (int i = 0; i < self->game.n_players; i++) {
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

x01_t* x01_new_game(int id, x01_options_t options, int max_rounds)
{

	x01_t* self = malloc(sizeof(x01_t));
	assert(self);
	game_init((game_t*)self, id, &cbs);
	self->players = NULL;
	self->options = options;
	self->round = 1;
	self->prev_score = 0;
	self->max_rounds = max_rounds;
	self->score = get_score(options);
	self->current_player = 0;
	self->darts = 0;
	for (int i = 0; i < N_SECTORS; i++) {
		self->sectors[i].shots = 0;
		self->sectors[i].enabled = 1;
	}
	for (int i = 0; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
	}

	return self;
}

void x01_delete(x01_t* self)
{
	game_delete((game_t*)self);
	free(self->players);
	free(self);
}

player_t* x01_get_player(x01_t* self, x01_player_t* x01_player)
{
	for (int i = 0; i < self->game.n_players; i++) {
		if (x01_player == &self->players[i]) {
			return &self->game.players[i];
		}
	}
	return NULL;
}
