#ifndef __GAME_MANAGER_H
#define __GAME_MANAGER_H

#include "game.h"

void game_manager_timer_cb(void* args);

void game_manager_init(void);
game_t* game_manager_new(game_ref_t game_ref, int options);
game_t* game_manager_get_by_id(int id);
game_t* game_manager_get_by_dartboard(int dartboard_id);
void game_manager_finish(game_t* game);

#endif // __GAME_MANAGER_H
