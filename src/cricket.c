#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cricket.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/* Function prototypes ********************************************************/

static bool player_all_closed(cricket_player_t* player);
static cricket_player_t* get_max_score(cricket_t* self);
static bool has_max_score(cricket_t* self, cricket_player_t* player);
static const char* zone_to_str(dartboard_zone_t zone);
static int get_mult_from_zone(int zone);
static bool valid_shot(dartboard_shot_t* ds);
static void gen_random_targets(int* targets, int len);
static void find_targets_to_gen(cricket_t* self);
static void gen_new_targets(cricket_t* self);
static bool number_closed(cricket_t* self, int num);

static void list_remove(int* list, int* len, int pos);
static int list_exists(int* list, int len, int val);

/* Callbacks ******************************************************************/
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

static bool has_max_score(cricket_t* self, cricket_player_t* player)
{
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].game_score > player->game_score) {
			return false;
		}
	}
	return true;
}

// TODO: to dartboard
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

// TODO: to dartboard
static int get_mult_from_zone(int zone)
{
	if (zone == ZONE_TRIPLE) {
		return 3;
	} else if (zone == ZONE_DOUBLE) {
		return 2;
	}
	return 1;
}

// TODO: to dartboard
// This function check if a shot is valid. This is:
// - Number must be between [0, 20]
// - Zone must be between [0, 4]
static bool valid_shot(dartboard_shot_t* ds)
{
	printf("NUMBER: %d, ZONE: %d\n", ds->number, ds->zone);
	return ds->number >= 0 && ds->number <= 20 && ds->zone >= 0 && ds->zone <= 4;
}

// This function generates random numbers for all possible targets.
// All numbers must be different.
static void gen_random_targets(int* targets, int len)
{
	// TODO: create fonctions for list manipulation (add, remove, etc)
	int vals[N_SECTORS];
	for (int i = 0; i < N_SECTORS; i++) {
		vals[i] = i;
	}
	int vals_len = N_SECTORS;
	for (int i = 0; i < len; i++) {
		int rand_num = rand() % vals_len;
		targets[i] = vals[rand_num];
		list_remove(vals, &vals_len, rand_num);
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
		for (int j = 0; j < self->n_players; j++) {
			if (self->players[j].shots[i] == 3) {
				change_closed = false;
			} else if (self->players[j].shots[i] != 0) {
				change_opened = true;
			}
		}
		if (change_closed && change_opened) {
			printf("%d has to be changed\n", self->enabled[i]);
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
	// Generate random numbers for postions == -1
	for (int i = 0; i < N_ENABLED; i++) {
		if (self->enabled[i] == -1) {
			int rand_num = rand() % targets_len;
			self->enabled[i] = targets[rand_num];
			list_remove(targets, &targets_len, rand_num);
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
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].shots[pos] != 3) {
			return false;
		}
	}
	return true;
}

static void list_remove(int* list, int* len, int pos)
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

void cricket_new_game(cricket_t* self, cricket_player_t* players, int n_players,
		cricket_options_t options, int max_score, int max_rounds)
{
	self->n_players = n_players;
	self->round = 1;
	self->max_rounds = max_rounds;
	self->max_score = max_score;
	self->current_player = 0;
	self->darts = 0;
	self->options = options;

	int en[N_ENABLED] = {20, 19, 18, 17, 16, 15, 0};
	memcpy(self->enabled, en, sizeof(en));

	srand(time(NULL));
	if (self->options & wild || self->options & crazy) {
		gen_random_targets(self->enabled, N_ENABLED);
	}
	self->players = players;
	for (int i = 0; i < n_players; i++) {
		self->players[i].game_score = 0;
		self->players[i].round_score = 0;
		self->players[i].marks = 0;
		memset(self->players[i].shots, 0, N_ENABLED * sizeof(int));
	}
	for (int i = 0; i < MAX_DARTS; i++) {
		self->dart_scores[i].number = -1;
		self->dart_scores[i].zone = -1;
	}
	printf("[Cricket] New game: %d players, %d rounds, max %d points\n",
				self->n_players, self->max_rounds, self->max_score);
}

cricket_player_t* cricket_check_finish(cricket_t* self)
{
	for (int i = 0; i < self->n_players; i++) {
		cricket_player_t* p = &self->players[i];
		if (player_all_closed(p) && (has_max_score(self, p))) {
			return p;
		}
	}
	if (self->round == self->max_rounds && self->darts == MAX_DARTS &&
			self->current_player == self->n_players - 1) {
		return get_max_score(self);
	}
	return NULL;
}

void cricket_next_player(cricket_t* self)
{
	self->current_player++;
	if (self->options & crazy) {
		printf("Numbers enabled before: \n");
		for (int i = 0; i < N_ENABLED; i++) {
			printf("%d, ", self->enabled[i]);
		}
		printf("\n");
		find_targets_to_gen(self);
		printf("Numbers enabled after: \n");
		for (int i = 0; i < N_ENABLED; i++) {
			printf("%d, ", self->enabled[i]);
		}
		printf("\n");
		gen_new_targets(self);
		printf("Numbers enabled new: \n");
		for (int i = 0; i < N_ENABLED; i++) {
			printf("%d, ", self->enabled[i]);
		}
		printf("\n");
	}
	if (self->current_player == self->n_players) {
		self->round++;
		if (self->round > self->max_rounds) {
			//TODO: esto en game
			cricket_check_finish(self);
		}
		self->current_player = 0;
	}
	self->darts = 0;
	self->players[self->current_player].round_score = 0;
}

bool cricket_new_dart(cricket_t* self, dartboard_shot_t* val)
{
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

	cricket_player_t* player = &self->players[self->current_player];
	printf("%s hit %s %d \n", player->p.name, zone_to_str(val->zone),
			val->number);
	int pos = list_exists(self->enabled, N_ENABLED, val->number);
	if (pos == -1 || number_closed(self, val->number)) {
		return false;
	}
	int mult = get_mult_from_zone(val->zone);
	for (int i = 0; i < mult; i++) {
		if (player->shots[pos] < 3) {
			player->shots[pos]++;
			player->marks++;
			if (player->shots[pos] == 3) {
				printf("%s closed %d\n", player->p.name, val->number);
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

const char* cricket_status(cricket_t* self)
{
	return json_helper_cricket_status(self);
}
