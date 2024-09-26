#include "array.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define REALLOC_CHUNK_SIZE 10

/* Global variables ***********************************************************/
/* Function prototypes ********************************************************/
/* Callbacks ******************************************************************/
/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

array_t* array_new(size_t init_size)
{
	array_t* self = malloc(sizeof(array_t));
	assert(self);
	if (init_size) {
		self->array = malloc(init_size * sizeof(void*));
		assert(self->array);
	} else {
		self->array = NULL;
	}
	self->total_size = init_size;
	self->used_size = 0;

	return self;
}

void array_append(array_t* self, void* new_item)
{
	if (self->used_size >= self->total_size) {
		self->total_size += REALLOC_CHUNK_SIZE;
		self->array = realloc(self->array, self->total_size * sizeof(void*));
		assert(self->array);
	}
	self->array[self->used_size++] = new_item;
}

void* array_get(array_t* self, unsigned long idx)
{
	if (idx >= self->used_size) {
		return NULL;
	}
	return self->array[idx];
}

void* array_remove(array_t* self, unsigned long idx)
{
	if (idx >= self->used_size) {
		return NULL;
	}
	
	void* removed_item = self->array[idx];
	for (unsigned long i = idx; i < self->used_size; i++) {
		self->array[idx] = self->array[idx+1];
	}
	self->used_size--;

	return removed_item;
}

void* array_pop(array_t* self)
{	
	return array_remove(self, self->used_size-1);
}

void array_free(array_t* self)
{
	free(self->array);
	free(self);
}
