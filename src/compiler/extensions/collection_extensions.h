#ifndef COLLECTION_EXTENSIONS
#define COLLECTION_EXTENSIONS
#include <stdlib.h>
#include "memory_extensions.h"

/// @brief Gets the count of the `collection` members.
/// @param collection The collection to count the members of.
/// @return The amount of members in `collection`.
size_t count_collection(void** collection);

/// @brief Copies the `source_collection`.
/// @param source_collection The collection to copy.
/// @return The copy of `source_collection`, or `NULL` if copying is unsuccessful.
void** copy_collection(void** source_collection);

#endif