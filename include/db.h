#ifndef __DB_H
#define __DB_H

#include <sqlite3.h>

#include "player.h"

typedef struct db_t {
	sqlite3* db;
	const char* filename;
} db_t;

void db_init();
int db_exec(db_t* db, char* query,
		int (*callback)(void *, int, char **, char **), void* user_data);
int db_players_new(player_t* new_player);
void db_players_get(player_t* player, const char* userid);
void db_cricket_new(int userid, float mpr);
void db_cricket_get();

#endif // __DB_H