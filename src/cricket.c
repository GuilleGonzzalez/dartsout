#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cricket.h"
#include "json_helper.h"

/* Global variables ***********************************************************/

static const int sector_values[N_SECTORS] = {25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

static int scoreables[7] = {20, 19, 18, 17, 16, 15, 0};

/* Function prototypes ********************************************************/

static bool player_all_closed(cricket_t* self, cricket_player_t* player);
static cricket_player_t* get_max_score(cricket_t* self);
static bool has_max_score(cricket_t* self, cricket_player_t* player);
static bool sector_enabled(cricket_t* self, int sector_number);
static const char* zone_to_str(dartboard_zone_t zone);
static bool valid_shot(dartboard_shot_t* ds);
static void gen_random_scoreables(int* generated, int len);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

// true if all ACTIVE numbers are closed
static bool player_all_closed(cricket_t* self, cricket_player_t* player)
{
	for (int i = 0; i < 7; i++) {
		if (player->shots[self->scoreables[i]] < 3) {
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

static bool sector_enabled(cricket_t* self, int sector_number)
{
	for (int i = 0; i < self->n_players; i++) {
		if (self->players[i].shots[sector_number] < 3) {
			return true;
		}
	}
	return false;
}

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
	printf("NUMBER: %d, ZONE: %d\n", ds->number, ds->zone);
	return ds->number >= 0 && ds->number <= 20 && ds->zone >= 0 && ds->zone <= 4;
}

static void gen_random_scoreables(int* generated, int len)
{
	int vals[N_SECTORS];
	for (int i = 0; i < N_SECTORS; i++) {
		vals[i] = i;
	}
	int vals_len = N_SECTORS;
	for (int i = 0; i < len; i++) {
		int rand_num = rand() % vals_len;
		generated[i] = vals[rand_num];
		for (int j = rand_num; j < vals_len-1; j++) {
			vals[j] = vals[j+1];
		}
		vals_len--;
	}
	for (int i = 0; i < len; i++) {
		printf("%d ", generated[i]);
	}
	printf("\n");
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
	
	srand(time(NULL));
	if (self->options & wild) {
		gen_random_scoreables(scoreables, 7);
	}

	self->scoreables = scoreables;

	self->players = players;
	for (int i = 0; i < n_players; i++) {
		self->players[i].game_score = 0;
		self->players[i].round_score = 0;
		memset(self->players[i].shots, 0, N_SECTORS * sizeof(int));
	}
	for (int i = 0; i < N_SECTORS; i++) {
		self->sectors[i].shots = 0;
		self->sectors[i].enabled = 0;
	}
	for (int i = 0; i < 7; i++) {
		self->sectors[self->scoreables[i]].enabled = 1;
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
		if (player_all_closed(self, p) && (has_max_score(self, p))) {
			return p;
		}
	}
	// If last round
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
		// Generar nuevos números aleatorios
		// Solo cambian los targets que están marcados
		// Si tienen todos los jugadores tienen 0 darts en ese número o alguien lo ha cerrado, no cambia.
		// la variable global (o la del objeto cricket_t) scoreables se actualiza.

		// Crear función con un parámetro que es un array
		// Esta función saca de vals (array con números 0-20) los números != -1 del array
		// Si es un número del 0-20, no se genera nuevo número.
		// Si es -1, generar número diferente al resto.
		// gen_new_scoreables([20, -1, 18, -1, -1 17, 0])
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
	bool scoreable = false;

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
	int mult = 1;
	if (val->zone == ZONE_TRIPLE) {
		mult = 3;
	} else if (val->zone == ZONE_DOUBLE) {
		mult = 2;
	}
	for (int i = 0; i < mult; i++) {
		if (!self->sectors[val->number].enabled) {
			return scoreable;
		}
		scoreable = true;
		if (player->shots[val->number] < 3) {
			player->shots[val->number]++;
			if (player->shots[val->number] == 3) {
				self->sectors[val->number].enabled =
						sector_enabled(self, val->number);
				printf("%s closed %d\n", player->p.name, val->number);
			}
		} else {
			if (!(self->options & no_score)) {
				player->game_score += sector_values[val->number];
				player->round_score += sector_values[val->number];
			}
		}
	}
	return scoreable;
}

const char* cricket_status(cricket_t* self)
{
	return json_helper_cricket_status(self);
}