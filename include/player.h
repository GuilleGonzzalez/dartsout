#ifndef __PLAYER_H
#define __PLAYER_H

typedef struct player_t {
	const char* userid;
	const char* name;
} player_t;

void player_init();

#endif // __PLAYER_H