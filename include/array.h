#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdlib.h>

typedef struct array_t {
	void** array;
	size_t used_size;
	size_t total_size;
} array_t;

array_t* array_new(size_t init_size);
void array_append(array_t* self, void* new_item);
void* array_get(array_t* self, unsigned long idx);
void* array_remove(array_t* self, unsigned long idx);
void* array_pop(array_t* self);
void array_free(array_t* self);

#endif // __ARRAY_H
