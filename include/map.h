#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include "../include/slice.h"

typedef struct MapNode {
	Slice *key;
	Slice *value;
} MapNode;

typedef struct {
	MapNode *list;	
	int len;
} MapState;

unsigned int Hash(Slice *input);

int MapSet(MapNode** map, size_t map_size, MapNode* node);

MapNode* MapGet(MapNode** map, size_t map_size, void* key, size_t key_size);
#endif
