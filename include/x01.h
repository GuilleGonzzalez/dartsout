#ifndef __X01_H
#define __X01_H

#include <stdbool.h>
#include "dartboard.h"

#define MAX_DARTS   3

typedef struct x01_player_t {
	const char* name;     // Player name
	int game_score;       // Number of "points" in the game
	int round_score;      // Number of "points" in the round
	int shots[N_SECTORS]; // Number of darts in each sector
} x01_player_t;

typedef struct x01_t {
	dartboard_sectors_t sectors[N_SECTORS];
	dartboard_shot_t dart_scores[MAX_DARTS]; // Scores of each shot
	x01_player_t* players;                   // Array of players
	int n_players;                           // Number of players
	int round;                               // Round number
	int max_rounds;                          // Max rounds number
	int score;                               // Game score (301, 501, etc.)
	int current_player;                      // Player which is playing
	int darts;                               // Number of darts thrown in this round
} x01_t;

void x01_new_game(x01_t* self, x01_player_t* players, int n_players, int score,
		int max_rounds);
x01_player_t* x01_check_finish(x01_t* self);
void x01_next_player(x01_t* self);
void x01_new_dart(x01_t* self, dartboard_shot_t* val);
const char* x01_status(x01_t* self);

#endif // __X01_H