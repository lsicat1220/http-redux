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

int GetMimeType(char* path, int size) {
	char* extension = memchr(path, '.', size);
	// Handling case of the period being at the end of the string
	if (extension - path == 12 - 1 ) {
		extension = NULL;
	} else {
		extension += 1;
	}
	// By now the path should have already been verified, so if no period is found there should still be a file
	if (extension == NULL) {
		return TXT;
	}
	int length = size - (extension - path);
	switch (length) {
		case 2:
			if (!(memcmp(extension, "js", 2))) {
				return JS;
			}
		case 3:
			if (!(memcmp(extension, "css", 3))) {
				return CSS;
			} else if (!(memcmp(extension, "txt", 3))) {
				return TXT;
			} else if (!(memcmp(extension, "ico", 3))) {
				return ICO;
			} else if (!(memcmp(extension, "png", 3))) {
				return PNG;
			} else if (!(memcmp(extension, "jpg", 3))) {
				return JPG;
			} 			
			break;
		case 4:
			if (!(memcmp(extension, "html", 4))) {
				return HTML;
			} else if (!(memcmp(extension, "json", 4))) {
				return JSON;
			}
	}	
	return UNSUPPORTED;
}
