#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct players_data_t {
	char* userid;
	char* name;
} players_data_t;

typedef struct cricket_data_t {
	char* userid;
	float mpr;
} cricket_data_t;

/* Global variables ***********************************************************/

static db_t dartsout_db;
static char* db_filename = "dartsout.db";

/* Function prototypes ********************************************************/

static int sql_cb(void* user_data, int argc, char** argv, char** az_col_name);
static int players_get_cb(void* user_data, int argc, char** argv,
		char** az_col_name);
static int cricket_get_cb(void* user_data, int argc, char** argv,
		char** az_col_name);

static bool file_exists(const char* filename);

/* Callbacks ******************************************************************/

static int sql_cb(void* user_data, int argc, char** argv, char** az_col_name)
{
	(void) user_data;
	for (int i = 0; i < argc; i++){
		printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int players_get_cb(void* user_data, int argc, char** argv,
		char** az_col_name)
{
	(void) az_col_name;
	printf("Players get callback\n");

	if (argc != 2) {
		perror("Invalid length\n");
	}

	players_data_t* data = user_data;
	data->userid = malloc(strlen(argv[0]));
	if (data->userid == NULL) {
		perror("CRITICAL ERROR");
	}
	strcpy(data->userid, argv[0]);
	data->name = malloc(strlen(argv[1]));
	if (data->name == NULL) {
		perror("CRITICAL ERROR");
	}
	strcpy(data->name, argv[1]);

	//TODO: free si no uso el jugador

	return 0;
}

static int cricket_get_cb(void* user_data, int argc, char** argv,
		char** az_col_name)
{
	(void) az_col_name;
	printf("Cricket get callback\n");

	if (argc != 2) {
		perror("Invalid length\n");
	}

	cricket_data_t* data = user_data;
	data->userid = argv[0];
	data->mpr = atof(argv[1]);
	printf("\n");

	return 0;
}

/* Function definitions *******************************************************/

static bool file_exists(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	bool exists = false;
	if (fp != NULL) {
		exists = true;
		fclose(fp);
	}
	return exists;
}


/* Public functions ***********************************************************/

void db_init()
{
	dartsout_db.filename = db_filename;

	if (file_exists(dartsout_db.filename)) {
		return;
	}

	char* query;
	query = "CREATE TABLE PLAYERS("
	"USERID CHAR(50) PRIMARY KEY NOT NULL,"
	"NAME CHAR(50) NOT NULL);";
	db_exec(&dartsout_db, query, sql_cb, NULL);
	query = "CREATE TABLE CRICKET("
	"USERID CHAR(50) PRIMARY KEY NOT NULL,"
	"MPR REAL NOT NULL);";
	db_exec(&dartsout_db, query, sql_cb, NULL);
}

int db_exec(db_t* db, char* query,
		int (*callback)(void *, int, char **, char **), void* user_data)
{
	int ret = 0;
	int rc;
	rc = sqlite3_open(db->filename, &db->db);
	if (rc) {
		printf("Can't open database: %s\n", sqlite3_errmsg(db->db));
		return -1;
	}
	char* err_msg = 0;
	rc = sqlite3_exec(db->db, query, callback, user_data, &err_msg);

	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		ret = -1;
	} else {
		printf("Query performed: %s\n", query);
	}
	rc = sqlite3_close(db->db);
	if (rc) {
		printf("Can't close database: %s\n", sqlite3_errmsg(db->db));
		return -1;
	}
	return ret;
}

int db_players_new(player_t* new_player)
{
	char* query;
	int ret = 0;
	// asprintf allocates memory and printf ea
	int len = asprintf(&query, "INSERT INTO PLAYERS (USERID, NAME) VALUES ('%s', '%s');",
			new_player->userid, new_player->name);
	if (len == -1) {
		perror("CRITICAL ERROR");
		exit(1);
	}
	ret = db_exec(&dartsout_db, query, NULL, NULL);
	free(query);

	return ret;
}

void db_players_get(player_t* player, const char* userid)
{
	char* query;
	// asprintf allocates memory and printf ea
	(void) userid;
	int len = asprintf(&query, "SELECT * FROM PLAYERS WHERE USERID='%s'", userid);
	if (len == -1) {
		perror("CRITICAL ERROR");
		exit(1);
	}
	players_data_t data;
	data.name = NULL;
	data.userid = NULL;
	int ret = db_exec(&dartsout_db, query, players_get_cb, &data);
	if (ret != 0 || data.name == NULL) {
		player->name = NULL;
		player->userid = NULL;
	} else {
		printf("USERID: %s\n", data.userid);
		printf("NAME: %s\n", data.name);
		player->name = data.name;
		player->userid = data.userid;

	}
}

void db_cricket_new(int userid, float mpr)
{
	char* query;
	// asprintf allocates memory and printf ea
	int len = asprintf(&query, "INSERT INTO CRICKET (USERID, MPR) VALUES (%d, %f);",
			userid, mpr);
	if (len == -1) {
		perror("CRITICAL ERROR");
		exit(1);
	}
	db_exec(&dartsout_db, query, NULL, NULL);
	free(query);
}


void db_cricket_get()
{
	char* query = "SELECT * FROM CRICKET";
	cricket_data_t data;
	db_exec(&dartsout_db, query, cricket_get_cb, &data);
	printf("USER: %s\n", data.userid);
	printf("MPR: %f\n", data.mpr);
}