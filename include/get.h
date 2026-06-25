#ifndef GET_C
#define GET_C

#include "../include/map.h"
#include "../include/parsing.h"

int CleanPath(char* file);

int PickSendType(char* file);

int RespondGet(RequestLine* req, MapState* map);

#endif
