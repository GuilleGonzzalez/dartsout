#ifndef __DARTBOARD_H
#define __DARTBOARD_H

#include <stdbool.h>

#include "player.h"

#define MAX_PLAYERS 4
#define N_SECTORS   21

typedef struct dartboard_sectors_t {
	int shots;
	bool enabled;
} dartboard_sectors_t;

typedef struct dartboard_shot_t {
	int number;
	int zone;
} dartboard_shot_t;

//TODO: single 0,1, double 2, triple 3
typedef enum {
	ZONE_TRIPLE,
	ZONE_DOUBLE,
	ZONE_SINGLE_INT,
	ZONE_SINGLE_EXT,
} dartboard_zone_t;

typedef enum {
	DARTBOARD_EVENT_REGISTER_PLAYER,
} dartboard_event_type_t;

typedef struct dartboard_event_t {
	dartboard_event_type_t type;
	union {
		player_t* player;
	};
} dartboard_event_t;

typedef struct dartboard_event_rsp_t {
	int ret_code;
	char* ret_str;
} dartboard_event_rsp_t;

void dartboard_init();
void dartboard_new_event(dartboard_event_t* event, dartboard_event_rsp_t* rsp);

#endif // __DARTBOARD_H