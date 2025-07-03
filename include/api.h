#ifndef __API_H
#define __API_H

typedef struct api_conn_t {
	struct mg_connection* c;
	int game_id;
} api_conn_t;

void api_init(void);
void api_fire(void);
void api_ws_write(const char* msg, int game_id);
void api_free(void);

#endif // __API_H
