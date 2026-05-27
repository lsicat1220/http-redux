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

int ParseVersion(Slice* version) {
	if (memcmp(version->start, "HTTP", 4) != 0 || version->len != 8) {
		fprintf(stderr, "ERROR: Invalid version format\n");
		return -1;
	}
	if (version->start[6] == '2') {
		return 2;
	} else if (version->start[8] == '0') {
		return 0;
	} else if (version->start[8] == '1') {
		return 1;
	}
	fprintf(stderr, "ERROR: Invalid version\n");
	return -1;
}

int ParseRequestLine(RequestLine* req, Slice* line) {
	Slice components[3];
	if (SplitSlice(line, components, 3, " ", 2) != 3) {
		fprintf(stderr, "ERROR: Invalid request line format");
		return -1;
	}
	req->method = ParseMethod(&components[0]);
	if (SliceToStr(&components[1], req->path, 128) != 0) {
		return -1;
	}	
	int version = ParseVersion(&components[2]);
	if (version < 0) {
		return -1;
	} else {
		req->version = version;
	}
	return 0;
}
