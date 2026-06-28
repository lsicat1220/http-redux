#ifndef GET_C
#define GET_C

#include "../include/map.h"
#include "../include/parsing.h"
#include <stdio.h>

int CleanPath(char* file, size_t max_len);

void WriteContent(int socket, FILE* file, long size, char* buffer, int buf_size); 

int RespondGet(RequestLine* req, MapState* map, int socket);

#endif
