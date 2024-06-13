#ifndef __GAME_H
#define __GAME_H

#include "dartboard.h"
#include "player.h"

typedef enum {
	GAME_CRICKET = 0,
	GAME_X01 = 1,
	// GAME_100 = 2,
	// GAME_ROUND_THE_WORLD = 3,
} game_games_t;

typedef enum {
	GAME_EVENT_STATUS,
	GAME_EVENT_NEW_GAME,
	GAME_EVENT_NEW_PLAYER,
	GAME_EVENT_NEXT_PLAYER,
	GAME_EVENT_NEW_DART,
	GAME_EVENT_FINISH_GAME,
} game_event_type_t;

typedef struct game_event_t {
	game_event_type_t type;
	union {
		dartboard_shot_t dart;
		player_t* player;
		struct {
			int game_id;
			int options;
		};
	};
} game_event_t;

typedef struct game_event_rsp_t {
	int ret_code;
	char* ret_str;
} game_event_rsp_t;

typedef struct game_t {
	game_games_t game;
	player_t players[MAX_PLAYERS];
	bool running;
	int n_players;
} game_t;

void game_init();
void game_new_event(game_event_t* event, game_event_rsp_t* rsp);
const char* game_status();

#endif // __GAME_H