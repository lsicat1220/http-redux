#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include "../include/slice.h"

typedef struct {
	void* key;
	void* value;
	size_t key_size;
	size_t value_size;
} mapNode;

unsigned int Hash(Slice *input);

int MapSet(mapNode** map, size_t map_size, mapNode* node);

mapNode* MapGet(mapNode** map, size_t map_size, void* key, size_t key_size);
#endif
