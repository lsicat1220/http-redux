#ifndef PARSING_H
#define PARSING_H
#include "../include/map.h"
#include "../include/slice.h"

enum contentType {
	HTML,
	CSS,
	JS,
	TXT,
	JSON,
	ICO,
	UNSUPPORTED,
};

enum methods {
	GET,
	POST,
};

typedef struct {
	int method;
	int version;
	char path[128];
} RequestLine;


int ParseHeaders(bufState* buf, MapState* map);

int ParseRequestLine(RequestLine* req, Slice* line);	

#endif
