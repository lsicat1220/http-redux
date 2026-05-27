#include "../include/parsing.h"
#include "../include/map.h"
#include "../include/slice.h"
#include <stdio.h>

int ParseHeader(MapState* map, Slice* header) {
	Slice components[2];
	if (SplitSlice(header, components, 2, ": ", 2) != 2) {
		fprintf(stderr, "ERROR: Invalid header format");
		return -1;
	}
	if (MapSet(&components[0], &components[1], map) != 0) {
		fprintf(stderr, "ERROR: Map set failed");
		return -1;
	} else {
		return 0;
	}
}
