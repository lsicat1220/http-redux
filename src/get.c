#include <stdio.h>
#include <string.h>
#include "../include/get.h"
#include "../include/parsing.h"
#include "../include/map.h"
#include "../include/utils.h"

#include "../include/defs.h"

enum contentType {
	HTML,
	CSS,
	JS,
	TXT,
	JSON,
	ICO,
	PNG,
	JPG,
	UNSUPPORTED,
};

int RespondGet(RequestLine* req, MapState* map) {
	char path[128];
	char response[8192];
	strncpy(path, req->path, 128);
	int file_status = CleanPath(path);
	if (file_status < 1) {
		fputs("ERROR: Invalid path", stderr);
		return -1;
	}	
	FILE* file = fopen(path, "r");
	int size = GetFileSize(file);
}
