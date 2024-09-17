#ifndef __GAME_H
#define __GAME_H

#include "dartboard.h"
#include "cricket.h"
#include "x01.h"

typedef enum {
	GAME_CRICKET = 0,
	GAME_X01 = 1,
	// GAME_100 = 2,
	// GAME_ROUND_THE_WORLD = 3,
} game_ref_t;

typedef enum {
	GAME_STATUS_DISABLED,
	GAME_STATUS_PLAYING,
} game_status_t;

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
		player_t player;
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
	game_ref_t game_ref;
	int id;
	bool running; // TODO: remove
	int options;
	int n_players;
	game_status_t status;
	union {
		cricket_t cricket;
		x01_t x01;
	};
	union {
		cricket_player_t cricket_players[MAX_PLAYERS];
		x01_player_t x01_players[MAX_PLAYERS];
	};
} game_t;

void game_init(game_t* game);
void game_new_event(game_t* game, game_event_t* event, game_event_rsp_t* rsp);
const char* game_status(game_t* game);
void game_reset(game_t* game);

#endif // __GAME_H
