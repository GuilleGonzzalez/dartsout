#ifndef __GAME_H
#define __GAME_H

#include "array.h"
#include "dartboard.h"
#include "player.h"
#include "array.h"

typedef enum {
	GAME_CRICKET = 0,
	GAME_X01 = 1,
	// GAME_100 = 2,
	// GAME_ROUND_THE_WORLD = 3,
} game_ref_t;

typedef enum {
	GAME_EVENT_STATUS,
	GAME_EVENT_NEW_GAME,
	GAME_EVENT_NEW_PLAYER,
	GAME_EVENT_NEXT_PLAYER,
	GAME_EVENT_NEW_DART,
	GAME_EVENT_BACK,
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

typedef struct game_t game_t;
typedef void (*game_start_cb_t)(game_t*);
typedef void (*game_next_player_cb_t)(game_t*);
typedef bool (*game_new_dart_cb_t)(game_t*, dartboard_shot_t*);
typedef const char* (*game_check_finish_cb_t)(game_t*, player_t**);
typedef const char* (*game_status_cb_t)(game_t*);
typedef void (*game_delete_cb_t)(game_t*);
typedef void* (*game_save_state_cb_t)(game_t*);
typedef bool (*game_restore_state_cb_t)(game_t*, void*);

typedef struct game_cbs_t {
	game_start_cb_t start_cb;
	game_new_dart_cb_t new_dart_cb;
	game_next_player_cb_t next_player_cb;
	game_check_finish_cb_t check_finish_cb;
	game_status_cb_t status_cb;
	game_delete_cb_t delete_cb;
	game_save_state_cb_t save_state_cb;
	game_restore_state_cb_t restore_state_cb;
} game_cbs_t;

struct game_t {
	game_ref_t game_ref;
	int id;
	bool running; // TODO: remove
	int options;
	player_t* players;
	int n_players;
	game_cbs_t* cbs;
	array_t* game_states;
};

void game_init(game_t* game, int id, game_cbs_t* cbs);
void game_new_event(game_t* game, game_event_t* event, game_event_rsp_t* rsp);
void game_finish(game_t* game);
void game_delete(game_t* game);

#endif // __GAME_H
