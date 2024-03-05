#ifndef __DARTBOARD_H
#define __DARTBOARD_H

#include <stdbool.h>

#define MAX_PLAYERS 4
#define MAX_DARTS   3
#define N_SECTORS   21

typedef struct dartboard_sectors_t {
	int shots;
	bool enabled;
} dartboard_sectors_t;

typedef struct dartboard_shot_t {
	int number;
	int zone;
} dartboard_shot_t;

typedef enum {
	ZONE_TRIPLE,
	ZONE_DOUBLE,
	ZONE_SINGLE_INT,
	ZONE_SINGLE_EXT,
} dartboard_zone_t;

void dartboard_init();

#endif // __DARTBOARD_H