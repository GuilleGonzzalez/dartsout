#ifndef __DB_H
#define __DB_H

#include <sqlite3.h>

typedef struct db_t {
	sqlite3* db;
	const char* name;
} db_t;

void db_init();
void db_exec(db_t* db, char* query);
void db_players_new(int player_uid, const char* name);
void db_players_get();
void db_cricket_new(int player_uid, float mpr);
void db_cricket_get();

#endif // __DB_H