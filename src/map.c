#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/map.h"
#include "../include/slice.h"

unsigned int Hash(Slice* input) {
	const unsigned int FNV_prime = 0x01000193;
	const unsigned int FNV_offset_basis = 0x811c9dc5;
	unsigned int hash = FNV_offset_basis;
	for (int i = 0; i < input->len; i++) {
		hash ^= input->start[i];
		hash *= FNV_prime;
	}	
	return hash;
}

int MapSet(Slice* key, Slice* value, MapState* state) {
	int map_size = state->len;
	unsigned int cursor = Hash(key) % map_size;
	unsigned int initial_index = cursor;
	MapNode* list = state->list;
	while (list[cursor].key) {
		Slice* current_key = list[cursor].key;
		if (current_key->len == key->len && !memcmp(current_key->start, key->start, key->len)) {
			break;
		} 
		cursor = (cursor + 1) % map_size;
		if (initial_index == cursor) {
			fprintf(stderr, "ERROR: Map is full\n");
			return -1;
		}
	}
	list[cursor].key = key;
	list[cursor].value = value;
	return 0;
}

mapNode* MapGet(mapNode** map, size_t map_size, void* key, size_t key_size) {
	unsigned int index = Hash(key, key_size) % map_size;
	unsigned int initial_index = index;
	while (map[index] != NULL) {
		if (map[index]->key_size == key_size && !memcmp(map[index]->key, key, key_size)) {
			return map[index];
		}
		index = (index + 1) % map_size;
		if (index == initial_index) {
			break;
		}
	}
	return NULL;
}
