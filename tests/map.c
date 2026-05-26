#include "../include/slice.h"
#include "../include/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BLOCK_SIZE 1024
#define NUM_NODES 64
#define NUM_SLICES 20
#define NUM_TRIALS 5

int main() {
	srand(time(NULL));
	MapNode list[NUM_NODES] = {0};
	Slice keys[NUM_SLICES] = {0};
	Slice values[NUM_SLICES];
	MapState map = {list, NUM_NODES};

	//generating random array of characters
	char block[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++) {
		block[i] = rand() % 128;
	}

	//setting map
	int cursor = 0;
	char* val;
	int len;
	for (int i = 0; i < NUM_SLICES; i++) {
		len = rand() % 20;	
		val = malloc(32);	
		keys[i].start = &block[cursor];
		keys[i].len = len;
		cursor += len;	
		sprintf(val, "This is value %d\n", i);
		values[i].start = val;
		values[i].len = strlen(values[i].start);
		MapSet(&keys[i], &values[i], &map);
	}

	//getting random values
	int target_index;
	Slice* result;
	for (int i = 0; i < NUM_TRIALS; i++) {
		target_index = rand() % NUM_SLICES;
		printf("Trial %d, index %d: ", i, target_index);
		fwrite(keys[target_index].start, sizeof(char), keys[target_index].len, stdout);
		printf("\n");
		result = MapGet(&map, &keys[target_index]);
		printf("Result: ");
		fwrite(result->start, sizeof(char), result->len, stdout);
		printf("\n\n");
	}

}
