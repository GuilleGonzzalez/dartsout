#ifndef __STATE_H
#define __STATE_H

typedef struct state_t state_t;

typedef void (*state_delete_cb_t)(state_t*);

struct state_t {
	state_delete_cb_t delete_cb;
};

#endif // __STATE_H
