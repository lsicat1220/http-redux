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

int MapSet(Slice *key, Slice *value, MapState *state);

Slice* MapGet(MapState* map, Slice* key);
#endif
