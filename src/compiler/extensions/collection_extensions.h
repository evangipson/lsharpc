#ifndef COLLECTION_EXTENSIONS
#define COLLECTION_EXTENSIONS
#include <stdlib.h>
#include "memory_extensions.h"

size_t count_collection(void** collection);

void** copy_collection(void** source_collection);

#endif