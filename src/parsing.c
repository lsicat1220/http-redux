#include "../include/parsing.h"
#include "../include/map.h"
#include "../include/slice.h"
#include <string.h>
#include <stdio.h>

int ParseHeader(MapState* map, Slice* header) {
	Slice components[2];
	if (SplitSlice(header, components, 2, ": ", 2) != 2) {
		fprintf(stderr, "ERROR: Invalid header format\n");
		return -1;
	}
	if (MapSet(&components[0], &components[1], map) != 0) {
		fprintf(stderr, "ERROR: Map set failed\n");
		return -1;
	} else {
		return 0;
	}
}

int ParseMethod(Slice* method) {
	switch (method->len) {
		//add methods as necessary
		case 3:
			if (memcmp(method->start, "GET", 3) == 0) {
				return GET;
			}
			break;
	}
	fprintf(stderr, "NONFATAL ERROR: Invalid or unsupported method\n");
	return UNSUPPORTED;
}

