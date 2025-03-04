#ifndef MEMORY_EXTENSIONS
#define MEMORY_EXTENSIONS
#include <stdio.h>
#include <stdlib.h>

void* safe_malloc(size_t size);

void safe_free(void* ptr);

void safe_free_collection(void** collection);

#endif