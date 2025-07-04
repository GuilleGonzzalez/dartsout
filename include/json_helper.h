#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cricket.h"
#include "x01.h"
#include "game.h"

const char* json_helper_simple_int(const char* str_id, int num);
const char* json_helper_simple_str(const char* str_id, const char* str);
const char* json_helper_last_dart(bool valid, int num, int zone);
const char* json_helper_winner(const char* name);
const char* json_helper_cricket_winner(cricket_t* cricket, const char* name);
const char* json_helper_cricket_status(cricket_t* cricket);
const char* json_helper_x01_status(x01_t* x01);
const char* json_helper_game_status(game_t* game);
const char* json_helper_send_game_id(int game_id);

int json_helper_new_player(const char* json_str, char* name, int name_len);
int json_helper_new_game(const char* json_str, int* game, int* options,
		player_t* players, int* n_players);
int json_helper_next_player(const char* json_str, int* board_id);
int json_helper_new_dart(const char* json_str, int* board_id, int* num,
		int* zone);
int json_helper_register_player(const char* json_str, char* userid,
		int userid_len, char* name, int name_len);
int json_helper_get_player(const char* json_str, char* userid,
		int userid_len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JSON_HELPER_H */