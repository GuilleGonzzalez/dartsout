#ifndef __API_H
#define __API_H

void api_init(void);
void api_fire(void);
void api_ws_write(const char* msg, int game_id);

typedef struct api_conn_t {
	struct mg_connection* c;
	int game_id;
} api_conn_t;

#endif // __API_H
