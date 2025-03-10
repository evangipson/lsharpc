#ifndef MEMORY_EXTENSIONS
#define MEMORY_EXTENSIONS
#include <stdio.h>
#include <stdlib.h>
#include "../logger/logger.h"

/// @brief Safely allocates memory of `size` bytes, or exits the program.
/// @param size The amount of bytes to allocate in memory.
/// @return A pointer which can be casted to a pointer of the expected type.
void* safe_malloc(size_t size);

/// @brief Safely frees the allocated memory of `ptr`.
/// @param ptr The pointer to free memory for.
void safe_free(void* ptr);

/// @brief Safely frees the allocated memory of `collection`.
/// @param collection The collection to free memory for.
void safe_free_collection(void** collection);

#endif