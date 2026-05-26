#include "../include/slice.h"
#include "../include/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define BLOCK_SIZE 2048
#define NUM_NODES 64
#define NUM_SLICES 64 
#define NUM_TRIALS 500
#define NUM_ITERATIONS 50

int iteration();

int main() {
	srand(time(NULL));
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		if (iteration() < 0) {
			printf("Failure on iteration %d\n", i);
			return -1;
		}
	}
	printf("All iterations pass\n");
}

int iteration() {
	MapNode list[NUM_NODES] = {0};
	Slice keys[NUM_SLICES] = {0};
	Slice values[NUM_SLICES];
	MapState map = {list, NUM_NODES};

	//generating random array of characters
	char block[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++) {
		block[i] = rand() % 96 + 32;
	}

	//setting map
	int cursor = 0;
	char* val;
	int len;
	for (int i = 0; i < NUM_SLICES; i++) {
		len = rand() % 10 + 10;	
		val = malloc(32);	
		if (!val) {
			fprintf(stderr, "ERROR: Malloc failure\n");
			return -2;
		}
		keys[i].start = &block[cursor];
		keys[i].len = len;
		cursor += len;	
		sprintf(val, "This is value %d\n", i);
		values[i].start = val;
		values[i].len = strlen(values[i].start);
		if (MapSet(&keys[i], &values[i], &map) == -1) {
			fprintf(stderr, "ERROR: Not enough map space%d\n", i);
			return -1;
		}
	}

	//getting random values
	int target_index;
	Slice* result;
	for (int i = 0; i < NUM_TRIALS; i++) {
		target_index = rand() % NUM_SLICES;
		result = MapGet(&map, &keys[target_index]);
		if (result != &values[target_index] && result != NULL) {
			fprintf(stderr, "ERROR: Value mismatch\n");
			printf("Trial %d, index %d: ", i, target_index);
			fwrite(keys[target_index].start, sizeof(char), keys[target_index].len, stdout);
			printf("\n");
			printf("Result: ");
			fwrite(result->start, sizeof(char), result->len, stdout);
			return -1;
		} else if (result == NULL) {
			fprintf(stderr, "ERROR: Key not found\n");
			return -1;
		}
	}

	Slice invalid_slice = {"No", 2};
	if (MapGet(&map, &invalid_slice) != NULL) {
		fprintf(stderr, "ERROR: Nonexistent key gives result\n");
		return -1;
	} 

	//overwrite test
	Slice* overwritten_key = &keys[0];
	char* new_val_str = malloc(32);
	if (!new_val_str) {
		fprintf(stderr, "ERROR: Malloc failure\n");
		return -2;
	}
	sprintf(new_val_str, "Overwrite\n");
	Slice new_val = {new_val_str, strlen(new_val_str)};
	if (MapSet(overwritten_key, &new_val, &map) == -1) {
		fprintf(stderr, "ERROR: Overwrite failed\n");
		return -1;
	}
	Slice* overwrite_result = MapGet(&map, overwritten_key);
	if (overwrite_result == NULL || overwrite_result != &new_val) {
		fprintf(stderr, "ERROR: Overwrite retrieval failed\n");
		return -1;
	}

	//clean up
	free(new_val_str);
	for (int i = 0; i < NUM_SLICES; i++) {
		free(values[i].start);
	}
	return 0;
}
