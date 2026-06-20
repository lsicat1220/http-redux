#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

void* TheMemmem(const void* needle, const void* haystack, size_t needlelen, size_t haystacklen);

int GetFileSize(FILE* file);

#endif
