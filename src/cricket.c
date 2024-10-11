#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cricket.h"
#include "dartboard.h"
#include "game.h"
#include "json_helper.h"
#include "log.h"
#include "player.h"
#include "utils.h"
#include "x01.h"

typedef struct cricket_state_t {
	state_t state;
	cricket_t game;
} cricket_state_t;

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static const int default_enabled[N_ENABLED] = {20, 19, 18, 17, 16, 15, 0};

/* Function prototypes ********************************************************/

static void start(game_t* game);
static void next_player(game_t* game);
static bool new_dart(game_t* game, dartboard_shot_t* val);
static const char* check_finish(game_t* game, player_t** winner_player);
static const char* status(game_t* self);
static void delete(game_t* game);
static state_t* save_state(game_t* game);
static bool restore_state(game_t* game, state_t* state);
static void delete_state(state_t* state);

static game_cbs_t cbs = {
	.start_cb = start,
	.next_player_cb = next_player,
	.new_dart_cb = new_dart,
	.check_finish_cb = check_finish,
	.status_cb = status,
	.delete_cb = delete,
	.save_state_cb = save_state,
	.restore_state_cb = restore_state,
};

static bool player_all_closed(cricket_player_t* player);
static player_t* get_max_score(cricket_t* self);
static bool has_max_score(cricket_t* self, cricket_player_t* player);
static void gen_random_targets(int* possible_targets, int possible_targets_len,
		int* targets, int targets_len);
static void find_targets_to_gen(cricket_t* self);
static void gen_all_targets_array(int* targets, int* len);
static void gen_new_targets(cricket_t* self);
static bool number_closed(cricket_t* self, int num);

static void list_remove(int* list, int* len, int val);
static void list_pop(int* list, int* len, int pos);
static int list_exists(int* list, int len, int val);

/* Callbacks ******************************************************************/

static void start(game_t* game)
{
	cricket_t* self = (cricket_t*)game;
	self->players = malloc(self->game.n_players * sizeof(cricket_player_t));
	assert(self->players);
	for (int i = 0; i < self->game.n_players; i++) {
		self->players[i].game_score = 0;
		self->players[i].round_score = 0;
		self->players[i].marks = 0;
		memset(self->players[i].shots, 0, N_ENABLED * sizeof(int));
	}

	LOG_INFO("New game: %d players, %d rounds, max %d points",
			self->game.n_players, self->max_rounds, self->max_score);
}

static void next_player(game_t* game)
{
	cricket_t* self = (cricket_t*)game;
	self->current_player++;
	if (self->options & crazy) {
		find_targets_to_gen(self);
		gen_new_targets(self);
	}
	if (self->current_player == self->game.n_players) {
		self->round++;
		self->current_player = 0;
	}

	self->darts = 0;
	self->players[self->current_player].round_score = 0;
}

static bool new_dart(game_t* game, dartboard_shot_t* val)
{
	cricket_t* self = (cricket_t*)game;
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

		
	LOG_INFO("%s hit %s %d", self->game.players[self->current_player].name,
			utils_zone_to_str(val->zone), val->number);
	int pos = list_exists(self->enabled, N_ENABLED, val->number);
	if (pos == -1 || number_closed(self, val->number)) {
		return false;
	}
	int mult = utils_mult_from_zone(val->zone);
	cricket_player_t* player = &self->players[self->current_player];
	for (int i = 0; i < mult; i++) {
		if (player->shots[pos] < 3) {
			player->shots[pos]++;
			player->marks++;
			if (player->shots[pos] == 3) {
				LOG_INFO("%s closed %d",
						self->game.players[self->current_player].name,
						val->number);
				if (number_closed(self, val->number)) {
					return true;
				}
			}
		} else {
			if (!(self->options & no_score)) {
				player->game_score += sector_values[val->number];
				player->round_score += sector_values[val->number];
				player->marks++;
			}
		}
	}
	return true;
}

static const char* check_finish(game_t* game, player_t** winner_player)
{
	cricket_t* self = (cricket_t*)game;

	for (int i = 0; i < self->game.n_players; i++) {
		if (player_all_closed(&self->players[i]) &&
				(has_max_score(self, &self->players[i]))) {
			*winner_player = &self->game.players[i];
			return json_helper_cricket_winner(self, (*winner_player)->name);
		}
	}
	if ((self->round > self->max_rounds) ||
			(self->round == self->max_rounds &&
			self->darts == MAX_DARTS &&
			self->current_player == self->game.n_players - 1)) {
		*winner_player = get_max_score(self);
		return json_helper_cricket_winner(self, (*winner_player)->name);
	}
	return NULL;
}

static const char* status(game_t* self)
{
	return json_helper_cricket_status((cricket_t*)self);
}

static void delete(game_t* game)
{
	cricket_t* self = (cricket_t*)game;

	free(self->players);
	free(self);
}

static state_t* save_state(game_t* game)
{
	cricket_t* self = (cricket_t*)game;

	cricket_state_t* current_state = malloc(sizeof(cricket_state_t));
	assert(current_state);
	current_state->state.delete_cb = delete_state;
	memcpy(&current_state->game, self, sizeof(cricket_t));
	current_state->game.players =
			malloc(sizeof(cricket_player_t) * self->game.n_players);
	assert(current_state->game.players);
	memcpy(current_state->game.players, self->players,
			sizeof(cricket_player_t) * self->game.n_players);

	return (state_t*)current_state;
}

static void delete_state(state_t* state)
{
	cricket_state_t* state_restore = (cricket_state_t*)state;
	LOG_INFO("Delete state");

	free(state_restore->game.players);
	free(state_restore);
}

static bool restore_state(game_t* game, state_t* state)
{
	cricket_t* self = (cricket_t*)game;
	cricket_state_t* self_to_restore = (cricket_state_t*)state;
	
	memcpy(self->players, self_to_restore->game.players,
			sizeof(cricket_player_t) * self->game.n_players);
	memcpy(self->dart_scores, self_to_restore->game.dart_scores,
			MAX_DARTS * sizeof(dartboard_shot_t));
	memcpy(self->enabled, self_to_restore->game.enabled, N_ENABLED * sizeof(int));
	self->round = self_to_restore->game.round;
	self->current_player = self_to_restore->game.current_player;
	self->darts = self_to_restore->game.darts;

	delete_state(state);

	return true;
}

/* Function definitions *******************************************************/

static bool player_all_closed(cricket_player_t* player)
{
	for (int i = 0; i < N_ENABLED; i++) {
		if (player->shots[i] < 3) {
			return false;
		}
	}
	return true;
}

static player_t* get_max_score(cricket_t* self)
{
	player_t* game_best_player = &self->game.players[0];
	cricket_player_t* best_player = &self->players[0];
	for (int i = 0; i < self->game.n_players; i++) {
		if (self->players[i].game_score >= best_player->game_score) {
			best_player = &self->players[i];
			game_best_player = &self->game.players[i];
		}
	}
	return game_best_player;
}

static bool has_max_score(cricket_t* self, cricket_player_t* player)
{
	for (int i = 0; i < self->game.n_players; i++) {
		if (self->players[i].game_score > player->game_score) {
			return false;
		}
	}
	return true;
}

// This function generates random numbers for a list of targets passed as param.
// All numbers must be different.
static void gen_random_targets(int* possible_targets, int possible_targets_len,
		int* targets, int targets_len)
{
	for (int i = 0; i < targets_len; i++) {
		int rand_num = rand() % possible_targets_len;
		targets[i] = possible_targets[rand_num];
		list_pop(possible_targets, &possible_targets_len, rand_num);
	}
}

// This function finds which numbers have to change.
// The requirements that must be met for the number to change are:
// - No one has closed it (no one has 3 targets)
// - Someone has opened it (someone has 1 or 2 targets)
static void find_targets_to_gen(cricket_t* self)
{
	for (int i = 0; i < N_ENABLED; i++) {
		bool change_closed = true;
		bool change_opened = false;
		for (int j = 0; j < self->game.n_players; j++) {
			if (self->players[j].shots[i] == 3) {
				change_closed = false;
			} else if (self->players[j].shots[i] != 0) {
				change_opened = true;
			}
		}
		if (change_closed && change_opened) {
			LOG_DEBUG("%d has to be changed", self->enabled[i]);
			self->enabled[i] = -1;
		}
	}
}

// This function generates an array with all possible targets
static void gen_all_targets_array(int* targets, int* len)
{
	for (int i = 0; i < *len; i++) {
		targets[i] = i;
	}
}

// This function generates the numbers of the enabled list that are marked as -1
// (has to be changed). New numbers cannot match those already on the list.
static void gen_new_targets(cricket_t* self)
{
	int targets[N_SECTORS];
	int targets_len = N_SECTORS;
	gen_all_targets_array(targets, &targets_len);
	// Remove from vals array numbers which we alredy have
	for (int i = 0; i < N_ENABLED; i++) {
		if (self->enabled[i] != -1) {
			list_remove(targets, &targets_len, self->enabled[i]);
		}
	}
	for (int i = 0; i < N_ENABLED; i++) {
		if (self->enabled[i] == -1) {
			int rand_num = rand() % targets_len;
			self->enabled[i] = targets[rand_num];
			list_remove(targets, &targets_len, targets[rand_num]);
		}
	}
}

// This function check if a given number is closed.
// For that, find the number position in the "enabled" array and check that
// all players have 3 targets on it.
// If the number does not exist in the "enabled" array, returns false
static bool number_closed(cricket_t* self, int num)
{
	int pos = -1;
	for (int i = 0; i < N_ENABLED; i++) {
		if (self->enabled[i] == num) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		return false;
	}
	for (int i = 0; i < self->game.n_players; i++) {
		if (self->players[i].shots[pos] != 3) {
			return false;
		}
	}
	return true;
}

static void list_remove(int* list, int* len, int val)
{
	for (int i = 0; i < *len; i++) {
		if (list[i] == val) {
			for (int j = i; j < *len; j++) {
				list[j] = list[j+1];
			}
			(*len)--;
			return;
		}
	}
	LOG_WARN("list_remove: value %d not found in list", val);
}

static void list_pop(int* list, int* len, int pos)
{
	(*len)--;
	for (int i = pos; i < *len; i++) {
		list[i] = list[i+1];
	}
}

static int list_exists(int* list, int len, int val)
{
	for (int i = 0; i < len; i++) {
		if (list[i] == val) {
			return i;
		}
	}
	return -1;
}

/* Public functions ***********************************************************/

cricket_t* cricket_new_game(int id, cricket_options_t options, int max_score,
		int max_rounds)
{
	cricket_t* self = malloc(sizeof(cricket_t));
	assert(self);
	game_init((game_t*)self, id, &cbs);
	self->players = NULL;
	self->options = options;
	self->round = 1;
	self->max_rounds = max_rounds;
	self->max_score = max_score;
	self->current_player = 0;
	self->darts = 0;

	memcpy(self->enabled, default_enabled, sizeof(default_enabled));

	srand(time(NULL));
	if (self->options & wild || self->options & crazy) {
		int all_targets[N_SECTORS];
		int all_targets_len = N_SECTORS;
		gen_all_targets_array(all_targets, &all_targets_len);
		gen_random_targets(all_targets, all_targets_len, self->enabled,
				N_ENABLED);
	}
	for (int i = 0; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
	}

	return self;
}

player_t* cricket_get_player(cricket_t* self, cricket_player_t* cricket_player)
{
	for (int i = 0; i < self->game.n_players; i++) {
		if (cricket_player == &self->players[i]) {
			return &self->game.players[i];
		}
	}
	return NULL;
}
