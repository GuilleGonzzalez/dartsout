#ifndef __GAME_H
#define __GAME_H

typedef enum {
	GAME_CRICKET = 0,
	GAME_X00 = 1,
	GAME_X01 = 2,
	GAME_15 = 3,
} game_games_t;

typedef enum {
	GAME_EVENT_NEW_GAME,
	GAME_EVENT_NEW_PLAYER,
	GAME_EVENT_NEW_DART,
} game_event_type_t;

typedef enum {
	ZONE_TRIPLE,
	ZONE_DOUBLE,
	ZONE_SINGLE_INT,
	ZONE_SINGLE_EXT,
} zone_t;

typedef struct dart_t {
	int num;
	zone_t zone;
} dart_t;

typedef struct game_event_t {
	game_event_type_t type;
	union {
		dart_t dart;
		char* player_name; //TODO: player_t player
		int game_id; //TODO
	};
} game_event_t;

void game_init();
char* game_new_event(game_event_t* event);

#endif // __GAME_H