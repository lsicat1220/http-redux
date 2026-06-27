#ifndef PARSING_H
#define PARSING_H
#include "../include/map.h"
#include "../include/slice.h"

enum methods {
	GET,
	POST,
};

typedef struct {
	int method;
	int version;
	Slice path;
} RequestLine;


int ParseHeaders(bufState* buf, MapState* map);

int ParseRequestLine(RequestLine* req, Slice* line);	

#endif
