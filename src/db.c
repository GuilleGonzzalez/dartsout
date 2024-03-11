#include "db.h"

#include <stdio.h>

/* Global variables ***********************************************************/

static db_t players_db;
static db_t cricket_db;

/* Function prototypes ********************************************************/

static int sql_cb(void* not_used, int argc, char** argv, char** az_col_name);

/* Callbacks ******************************************************************/

static int sql_cb(void* not_used, int argc, char** argv, char** az_col_name)
{
	(void)not_used;
	for (int i = 0; i < argc; i++){
		printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void db_init()
{
	char* query;

	//TODO: si no existe el fichero, crear bases de datos

	players_db.name = "players.db";
	query = "CREATE TABLE PLAYERS("
	"UISERD CHAR(50) PRIMARY KEY NOT NULL,"
	"NAME CHAR(50) NOT NULL);";
	db_exec(&players_db, query);

	cricket_db.name = "cricket.db";
	query = "CREATE TABLE CRICKET("
	"USERID CHAR(50) INT PRIMARY KEY NOT NULL,"
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
	char* err_msg = 0;
	rc = sqlite3_exec(db->db, query, sql_cb, 0, &err_msg);
	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
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

int db_players_new(player_t* new_player)
{
	int n_players = 0;
	player_t players;
	//TODO: an array of players
	db_players_get(&players, &n_players);
	for (int i = 0; i < n_players; i++) {
		// if (players[i].name == new_player->name) {
			// return -1;
		// }
	}

	char query[100];
	sprintf(query, "INSERT INTO PLAYERS (USERID, NAME) VALUES (%s, '%s');",
			new_player->userid, new_player->name);
	db_exec(&players_db, query);

	return 0;
}

void db_players_get(player_t* player, int* n_players)
{
	(void)player;
	(void)n_players;
	char* query = "SELECT * FROM PLAYERS";
	db_exec(&players_db, query);
}

void db_cricket_new(int userid, float mpr)
{
	char query[100];
	sprintf(query, "INSERT INTO CRICKET (USERID, MPR) VALUES (%d, %f);",
			userid, mpr);
	db_exec(&cricket_db, query);
}

void db_cricket_get()
{
	char* query = "SELECT * FROM CRICKET";
	db_exec(&cricket_db, query);
}