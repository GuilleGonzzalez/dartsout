#ifndef __X01_H
#define __X01_H

#include <stdbool.h>
#include "dartboard.h"
#include "player.h"

#define MAX_DARTS   3

typedef struct x01_player_t {
	player_t p;           // Player
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
	x1001      = (1 << 2),
};

typedef int x01_options_t;

typedef struct x01_t {
	dartboard_sectors_t sectors[N_SECTORS];
	dartboard_shot_t dart_scores[MAX_DARTS]; // Scores of each shot
	x01_player_t* players;                   // Array of players
	x01_options_t options;                   // X01 options
	int n_players;                           // Number of players
	int round;                               // Round number
	int prev_score;                          // Score at the begin of the round
	int max_rounds;                          // Max rounds number
	int score;                               // Game score (301, 501, etc.)
	int current_player;                      // Player which is playing
	int darts;                               // Number of darts thrown in this round
} x01_t;

void x01_new_game(x01_t* self, x01_player_t* players, int n_players,
		x01_options_t options, int max_rounds);
x01_player_t* x01_check_finish(x01_t* self);
void x01_next_player(x01_t* self);
bool x01_new_dart(x01_t* self, dartboard_shot_t* val);
const char* x01_status(x01_t* self);

#endif // __X01_H