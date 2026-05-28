#include "../include/parsing.h"
#include "../include/slice.h"
#include <stdio.h>

int main() {
	char tests[6][128] = {
		"GET /api/login HTTP/1.1",
		"GET /api/login HTTP/1.0",
		"GET /api/login HTTP/2.0",
		"This doesn't work",
		"This also doesn't work",
		"Thisdoesn'twork"
	};
	RequestLine req;
	Slice slice;
	for (int i = 0; i < 6; i++) {
		printf("Trial %d: %s\n", i + 1, tests[i]);
		StrToSlice(tests[i], &slice);	
		if (ParseRequestLine(&req, &slice) == 0) {
			printf("Method: %d\n", req.method);
			printf("Path: %s\n", req.path);
			printf("Version: %d\n", req.version);
		} else {
			printf("Parsing error\n");
		}
		printf("\n");
	}
}
