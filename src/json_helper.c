#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "json_helper.h"

/* Global variables ***********************************************************/
/* Function prototypes ********************************************************/

static const char* json_get_string(cJSON* json, const char* id);
static int json_get_int(int* num, cJSON* json, char* id);
// static int json_get_float(float* num, cJSON* json, char* id);
// static int json_get_bool(bool* bl, cJSON* json, char* id);

/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/

static const char* json_get_string(cJSON* json, const char* id)
{
	cJSON* res = cJSON_GetObjectItemCaseSensitive(json, id);
	if (cJSON_IsString(res)) {
		return res->valuestring;
	}
	return NULL;
}

static int json_get_int(int* num, cJSON* json, char* id)
{
	cJSON* res = cJSON_GetObjectItem(json, id);
	if (cJSON_IsNumber(res)) {
		*num = res->valueint;
		return 0;
	}
	return 1;
}

// static int json_get_float(float* num, cJSON* json, char* id)
// {
// 	cJSON* res = cJSON_GetObjectItem(json, id);
// 	if (cJSON_IsNumber(res)) {
// 		*num = (float)res->valuedouble;
// 		return 0;
// 	}
// 	return 1;
// }

// static int json_get_bool(bool* bl, cJSON* json, char* id)
// {
// 	cJSON* res = cJSON_GetObjectItem(json, id);
// 	if (cJSON_IsBool(res)) {
// 		*bl = cJSON_IsTrue(res);
// 		return 0;
// 	}
// 	return 1;
// }

/* Public functions ***********************************************************/

const char* json_helper_simple_int(const char* str_id, int num)
{
	char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, str_id, num);
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_simple_str(const char* str_id, const char* str)
{
	char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, str_id, str);
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_cricket_status(cricket_t* cricket)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", 1); //TODO: hardcoded (cricket msg)
	cJSON_AddNumberToObject(json, "n_players", cricket->n_players);
	cJSON_AddNumberToObject(json, "round", cricket->round);
	cJSON_AddNumberToObject(json, "max_rounds", cricket->max_rounds);
	cJSON_AddNumberToObject(json, "max_score", cricket->max_score);
	cJSON_AddNumberToObject(json, "current_player", cricket->current_player);
	cJSON_AddNumberToObject(json, "darts", cricket->darts);
	cJSON* dart_scores = cJSON_AddArrayToObject(json, "dart_scores");
	for (int i = 0; i < MAX_DARTS; i++) {
		dartboard_shot_t ds = cricket->dart_scores[i];
		cJSON* dart_score = cJSON_CreateObject();
		cJSON_AddNumberToObject(dart_score, "num", ds.number);
		cJSON_AddNumberToObject(dart_score, "zone", ds.zone);
		cJSON_AddItemToArray(dart_scores, dart_score);
	}
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < cricket->n_players; i++) {
		cricket_player_t p = cricket->players[i];
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "name", p.p.name);
		cJSON_AddNumberToObject(player, "game_score", p.game_score);
		cJSON_AddNumberToObject(player, "round_score", p.round_score);
		cJSON* shots = cJSON_CreateArray();
		for (int j = 0; j < N_SECTORS; j++) {
			cJSON_AddItemToArray(shots, cJSON_CreateNumber(p.shots[j]));
		}
		cJSON_AddItemToObject(player, "shots", shots);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_x01_status(x01_t* x01)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", 2); //TODO: hardcoded (x01 msg)
	cJSON_AddNumberToObject(json, "score", x01->score);
	cJSON_AddNumberToObject(json, "n_players", x01->n_players);
	cJSON_AddNumberToObject(json, "round", x01->round);
	cJSON_AddNumberToObject(json, "max_rounds", x01->max_rounds);
	cJSON_AddNumberToObject(json, "current_player", x01->current_player);
	cJSON_AddNumberToObject(json, "darts", x01->darts);
	cJSON* dart_scores = cJSON_AddArrayToObject(json, "dart_scores");
	for (int i = 0; i < MAX_DARTS; i++) {
		dartboard_shot_t ds = x01->dart_scores[i];
		cJSON* dart_score = cJSON_CreateObject();
		cJSON_AddNumberToObject(dart_score, "num", ds.number);
		cJSON_AddNumberToObject(dart_score, "zone", ds.zone);
		cJSON_AddItemToArray(dart_scores, dart_score);
	}
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < x01->n_players; i++) {
		x01_player_t p = x01->players[i];
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "name", p.name);
		cJSON_AddNumberToObject(player, "game_score", p.game_score);
		cJSON_AddNumberToObject(player, "round_score", p.round_score);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_game_status(game_t* game)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", 0); //TODO:  (status msg)
	cJSON_AddBoolToObject(json, "running", game->running);
	cJSON_AddNumberToObject(json, "game_id", game->game);
	cJSON_AddNumberToObject(json, "n_players", game->n_players);
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < game->n_players; i++) {
		player_t p = game->players[i];
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "userid", p.userid);
		cJSON_AddStringToObject(player, "name", p.name);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_reg_player(player_t* player)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", 2); //TODO:  (reg player msg)
	cJSON_AddStringToObject(json, "userid", player->userid);
	cJSON_AddStringToObject(json, "name", player->name);
	out = cJSON_Print(json);
	cJSON_Delete(json);
	return out;
}

int json_helper_new_player(const char* json_str, char* name, int name_len)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	const char* str;
	int err = 0;
	str = json_get_string(json, "name");
	if (str == NULL) {
		cJSON_Delete(json);
		return 1;
	}
	strncpy(name, str, name_len);
	cJSON_Delete(json);
	return err;
}

int json_helper_new_game(const char* json_str, int* game)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	int err = 0;
	err |= json_get_int((int*)(game), json, "game");
	cJSON_Delete(json);
	return err;
}

int json_helper_new_dart(const char* json_str, int* board_id, int* num,
		int* zone)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	int err = 0;
	err |= json_get_int((int*)(board_id), json, "board_id");
	err |= json_get_int((int*)(num), json, "num");
	err |= json_get_int((int*)(zone), json, "zone");
	cJSON_Delete(json);
	return err;
}

int json_helper_register_player(const char* json_str, char* userid,
		int userid_len, char* name, int name_len)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	const char* str;
	int err = 0;
	str = json_get_string(json, "userid");
	if (str == NULL) {
		cJSON_Delete(json);
		return 1;
	}
	strncpy(userid, str, userid_len);
	str = json_get_string(json, "name");
	if (str == NULL) {
		cJSON_Delete(json);
		return 1;
	}
	strncpy(name, str, name_len);
	cJSON_Delete(json);
	return err;
}

int json_helper_get_player(const char* json_str, char* userid,
		int userid_len)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	const char* str;
	int err = 0;
	str = json_get_string(json, "userid");
	if (str == NULL) {
		cJSON_Delete(json);
		return 1;
	}
	strncpy(userid, str, userid_len);
	cJSON_Delete(json);
	return err;
}