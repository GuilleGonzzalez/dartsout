#ifndef __X01_H
#define __X01_H

#include <stdbool.h>
#include "dartboard.h"
#include "game.h"

#define MAX_DARTS   3

typedef struct x01_player_t {
	int game_score;       // Number of "points" in the game
	int round_score;      // Number of "points" in the round
	int shots[N_SECTORS]; // Number of darts in each sector
} x01_player_t;

enum {
	double_in  = (1 << 0),
	double_out = (1 << 1),
	x301       = (1 << 2),
	x501       = (1 << 3),
	x701       = (1 << 4),
	x901       = (1 << 5),
	x1001      = (1 << 6),
};

typedef int x01_options_t;

typedef struct x01_t {
	game_t game;
	x01_player_t* players;
	dartboard_sectors_t sectors[N_SECTORS];
	dartboard_shot_t dart_scores[MAX_DARTS]; // Scores of each shot
	x01_options_t options;                   // X01 options
	int round;                               // Round number
	int prev_score;                          // Score at the begin of the round
	int max_rounds;                          // Max rounds number
	int score;                               // Game score (301, 501, etc.)
	int current_player;                      // Player which is playing
	int darts;                               // Number of darts thrown in this round
} x01_t;

// typedef struct x01_state_t {

// } x01_state_t;

x01_t* x01_new_game(x01_options_t options, int max_rounds);
void x01_delete(x01_t* self);
player_t* x01_get_player(x01_t* self, x01_player_t* x01_player);

#endif // __X01_H
