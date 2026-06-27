#include "../include/parsing.h"
#include "../include/map.h"
#include "../include/slice.h"
#include <string.h>
#include <stdio.h>

int ParseHeaders(bufState* buf, MapState* map) {
	Slice currSlice;
	Slice segments[2];
	while (buf->unprocessed_offset < buf->used_bytes) {
		TokenizeSlice(buf, &currSlice, "\r\n", 2);
		if (currSlice.len == 0) {
			break;
		}
		if (SplitSlice(&currSlice, segments, 2, ":", 1) < 0) {
			fprintf(stderr, "ERROR: Splitting slice failed\n");
			return -1;
		}
		TrimSlice(&segments[0]);
		TrimSlice(&segments[1]);
		if (MapSet(&segments[0], &segments[1], map)) {
			fprintf(stderr, "ERROR: Setting to map failed\n");
			return -1;
		}
	}	
	return 0;
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
	return -1;
}

int ParseVersion(Slice* version) {
	if (memcmp(version->start, "HTTP", 4) != 0 || version->len != 8) {
		fprintf(stderr, "ERROR: Invalid version format\n");
		return -1;
	}
	if (version->start[5] == '2') {
		return 2;
	} else if (version->start[7] == '0') {
		return 0;
	} else if (version->start[7] == '1') {
		return 1;
	}
	fprintf(stderr, "ERROR: Invalid version\n");
	return -1;
}

int ParseRequestLine(RequestLine* req, Slice* line) {
	Slice components[3];
	int num_slices = SplitSlice(line, components, 3, " ", 1);
	if (num_slices != 3) {
		fprintf(stderr, "ERROR: Invalid request line format: number of slices = %d\n", num_slices);
		return -1;
	}
	req->method = ParseMethod(&components[0]);
	req->path = components[1];
	int version = ParseVersion(&components[2]);
	if (version < 0) {
		return -1;
	} else {
		req->version = version;
	}
	return 0;
}
