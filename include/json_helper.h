#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

char* json_helper_simple_int(const char* str_id, int num);
char* json_helper_simple_str(const char* str_id, const char* str);

int json_helper_new_player(const char* json_str, char* player, int player_len);
int json_helper_new_game(const char* json_str, int* game);
int json_helper_new_dart(const char* json_str, int* board_id, int* num,
		int* zone);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JSON_HELPER_H */