#ifndef __PLAYER_H
#define __PLAYER_H

typedef struct player_t {
	const char* userid;
	const char* name;
	int dartboard_id;
} player_t;

void player_init(void);

#endif // __PLAYER_H
