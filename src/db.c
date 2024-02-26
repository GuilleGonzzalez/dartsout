#include "db.h"

#include <stdio.h>

/* Global variables ***********************************************************/

static db_t players_db;
static db_t cricket_db;

/* Function prototypes ********************************************************/

static int sql_cb(void* NotUsed, int argc, char** argv, char** azColName);

/* Callbacks ******************************************************************/

static int sql_cb(void* NotUsed, int argc, char** argv, char** azColName)
{
	(void)NotUsed;
	for (int i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void db_init()
{
	char* query;

	players_db.name = "players.db";
	query = "CREATE TABLE PLAYERS("
	"UID INT PRIMARY KEY NOT NULL,"
	"NAME CHAR(50) NOT NULL);";
	db_exec(&players_db, query);

	cricket_db.name = "cricket.db";
	query = "CREATE TABLE CRICKET("
	"UID INT PRIMARY KEY NOT NULL,"
	"MPR REAL NOT NULL);";
	db_exec(&cricket_db, query);
}

void db_exec(db_t* db, char* query)
{
	int rc;
	rc = sqlite3_open(db->name, &db->db);
	if (rc) {
		printf("Can't open database: %s\n", sqlite3_errmsg(db->db));
		return;
	} else {
		printf("Opened database successfully\n");
	}
	char* zErrMsg = 0;
	rc = sqlite3_exec(db->db, query, sql_cb, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		printf("Query performed: %s\n", query);
	}
	rc = sqlite3_close(db->db);
	if (rc) {
		printf("Can't close database: %s\n", sqlite3_errmsg(db->db));
		return;
	} else {
		printf("Closed database successfully\n");
	}
}

void db_players_new(int player_uid, const char* name)
{
	char query[100];
	sprintf(query, "INSERT INTO PLAYERS (UID, NAME) VALUES (%d, '%s');",
			player_uid, name);
	db_exec(&players_db, query);
}

void db_players_get()
{
	char* query = "SELECT * FROM PLAYERS";
	db_exec(&players_db, query);
}

void db_cricket_new(int player_uid, float mpr)
{
	char query[100];
	sprintf(query, "INSERT INTO CRICKET (UID, MPR) VALUES (%d, %f);",
			player_uid, mpr);
	db_exec(&cricket_db, query);
}

void db_cricket_get()
{
	char* query = "SELECT * FROM CRICKET";
	db_exec(&cricket_db, query);
}