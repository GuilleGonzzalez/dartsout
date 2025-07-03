#include <assert.h>
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <string.h>

#include "json_helper.h"
#include "cricket.h"
#include "game.h"
#include "log.h"
#include "player.h"
#include "x01.h"

#define MSG_ID_STATUS    0
#define MSG_ID_CRICKET   1
#define MSG_ID_X01       2
#define MSG_ID_LAST_DART 3
#define MSG_ID_WINNER    4
#define MSG_ID_GAME_ID   5

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
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_simple_str(const char* str_id, const char* str)
{
	char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, str_id, str);
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_last_dart(bool valid, int num, int zone)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_LAST_DART);
	cJSON_AddBoolToObject(json, "valid", valid);
	cJSON_AddNumberToObject(json, "num", num);
	cJSON_AddNumberToObject(json, "zone", zone);
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

// Not cricket winner
const char* json_helper_winner(const char* name)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_WINNER);
	cJSON_AddStringToObject(json, "name", name);
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_cricket_winner(cricket_t* cricket, const char* name)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_WINNER);
	cJSON_AddNumberToObject(json, "game_id", GAME_CRICKET);
	cJSON_AddStringToObject(json, "name", name);
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < cricket->game.n_players; i++) {
		cricket_player_t* cricket_p = &cricket->players[i];
		player_t* p = cricket_get_player(cricket, cricket_p);
		int mpr = (int)((float)cricket_p->marks / cricket->round * 100.0);
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "name", p->name);
		cJSON_AddNumberToObject(player, "game_score", cricket_p->game_score);
		cJSON_AddNumberToObject(player, "mpr", mpr);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_cricket_status(cricket_t* cricket)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_CRICKET);
	cJSON_AddNumberToObject(json, "n_players", cricket->game.n_players);
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
	cJSON* enabled = cJSON_CreateArray();
	for (int i = 0; i < N_ENABLED; i++) {
		cJSON_AddItemToArray(enabled, cJSON_CreateNumber(cricket->enabled[i]));
	}
	cJSON_AddItemToObject(json, "enabled", enabled);
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < cricket->game.n_players; i++) {
		cricket_player_t* cricket_p = &cricket->players[i];
		player_t* p = cricket_get_player(cricket, cricket_p);
		assert(p);
		int mpr = (int)((float)cricket_p->marks / cricket->round * 100.0);
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "name", p->name);
		cJSON_AddNumberToObject(player, "game_score", cricket_p->game_score);
		cJSON_AddNumberToObject(player, "round_score", cricket_p->round_score);
		cJSON_AddNumberToObject(player, "mpr", mpr);
		cJSON_AddStringToObject(player, "img_path", "res/user.svg");
		cJSON* shots = cJSON_CreateArray();
		for (int j = 0; j < N_ENABLED; j++) {
			cJSON_AddItemToArray(shots, cJSON_CreateNumber(cricket_p->shots[j]));
		}
		cJSON_AddItemToObject(player, "shots", shots);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_x01_status(x01_t* x01)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_X01);
	cJSON_AddNumberToObject(json, "score", x01->score);
	cJSON_AddNumberToObject(json, "n_players", x01->game.n_players);
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
	for (int i = 0; i < x01->game.n_players; i++) {
		x01_player_t* x01_p = &x01->players[i];
		cJSON* player = cJSON_CreateObject();
		player_t* p = x01_get_player(x01, x01_p);
		assert(p);
		cJSON_AddStringToObject(player, "name", p->name);
		cJSON_AddNumberToObject(player, "game_score", x01_p->game_score);
		cJSON_AddNumberToObject(player, "round_score", x01_p->round_score);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_game_status(game_t* game)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_STATUS);
	cJSON_AddBoolToObject(json, "running", game->running);
	cJSON_AddNumberToObject(json, "game_id", game->game_ref);
	cJSON_AddNumberToObject(json, "options", game->options);
	cJSON_AddNumberToObject(json, "n_players", game->n_players);
	cJSON* players = cJSON_AddArrayToObject(json, "players");
	for (int i = 0; i < game->n_players; i++) {
		cJSON* player = cJSON_CreateObject();
		cJSON_AddStringToObject(player, "userid", game->players[i].userid);
		cJSON_AddStringToObject(player, "name", game->players[i].name);
		cJSON_AddItemToArray(players, player);
	}
	out = cJSON_PrintUnformatted(json);
	cJSON_Delete(json);
	return out;
}

const char* json_helper_send_game_id(int game_id)
{
	const char* out;
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "msg_id", MSG_ID_GAME_ID);
	cJSON_AddNumberToObject(json, "game_id", game_id);
	out = cJSON_PrintUnformatted(json);
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

int json_helper_new_game(const char* json_str, int* game, int* options,
		player_t* players, int* n_players)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	int err = 0;
	err |= json_get_int((int*)(game), json, "game");
	err |= json_get_int((int*)(options), json, "options");

	// TODO: json_get_list
	cJSON* players_obj = cJSON_GetObjectItem(json, "players");
	if (players_obj == NULL || !cJSON_IsArray(players_obj)) {
		cJSON_Delete(json);
		return 1;
	}

	*n_players = cJSON_GetArraySize(players_obj);
	for (int i = 0; i < *n_players; i++) {
		cJSON *player = cJSON_GetArrayItem(players_obj, i);
		cJSON *name_item = cJSON_GetObjectItem(player, "name");
		cJSON *board_id_item = cJSON_GetObjectItem(player, "board_id");
		const char* name = name_item ? name_item->valuestring : "Desconocido";
		int board_id = board_id_item ? board_id_item->valueint : -1;
		players[i].name = strdup(name);
		players[i].dartboard_id = board_id;
		LOG_TRACE("Player %d: %s (0x%X)", i + 1, name, board_id);
	}

	cJSON_Delete(json);
	return err;
}

int json_helper_next_player(const char* json_str, int* board_id)
{
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		return 1;
	}
	int err = 0;
	err |= json_get_int((int*)(board_id), json, "board_id");
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
