#ifndef __GAME_H
#define __GAME_H

typedef enum {
	GAME_EVENT_NEW_DART,
	GAME_EVENT_NEW_GAME, //TODO
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
	};
} game_event_t;

void game_init();
void game_new_event(game_event_t* l_event);
void game_fire();

#endif // __GAME_H