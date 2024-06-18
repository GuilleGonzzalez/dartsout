#ifndef __CRICKET_H
#define __CRICKET_H

#include <stdbool.h>
#include "dartboard.h"
#include "player.h"

#define MAX_DARTS   3

typedef struct cricket_player_t {
	player_t p;           // Player
	int game_score;       // Number of "points" in the game
	int round_score;      // Number of "points" in the round
	int shots[N_SECTORS]; // Number of darts in each sector
} cricket_player_t;

enum {
	no_score   = (1 << 0),
	cut_throat = (1 << 1),
	wild       = (1 << 2), // Random numbers
	crazy      = (1 << 3), // Los numeros cambian en cada ronda, a menos que se cierre
	wild_crazy = (1 << 4), // Combinación de ambos
};

typedef int cricket_options_t;

typedef struct cricket_t {
	dartboard_sectors_t sectors[N_SECTORS];  // All dartboard sectors
	dartboard_shot_t dart_scores[MAX_DARTS]; // Scores of each shot
	cricket_player_t* players; // Pointer to an array of players
	cricket_options_t options; // Cricket options
	int* scoreables;           // Scoreables numbers // TODO: cluld not be 7
	int n_players;             // Number of players
	int round;                 // Round number
	int max_rounds;            // Max rounds number
	int max_score;             // Max score
	int current_player;        // Player which is playing
	int darts;                 // Number of darts thrown in this round
} cricket_t;

void cricket_new_game(cricket_t* self, cricket_player_t* players, int n_players,
		cricket_options_t options, int max_score, int max_rounds);
cricket_player_t* cricket_check_finish(cricket_t* self);
void cricket_next_player(cricket_t* self);
bool cricket_new_dart(cricket_t* self, dartboard_shot_t* val);
const char* cricket_status(cricket_t* self);

#endif // __CRICKET_H