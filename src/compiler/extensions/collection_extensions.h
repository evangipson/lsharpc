#ifndef COLLECTION_EXTENSIONS
#define COLLECTION_EXTENSIONS
#include <stdlib.h>
#include "../debug/logger.h"
#include "memory_extensions.h"
#include "string_extensions.h"

/// @brief A type definition for a function to provide `copy_collection` to handle copying various types.
typedef void* (*copy_function)(const void*);

/// @brief Gets the count of the `collection` members. Relies on a null-terminated member at the end of the collection.
/// @param collection The collection to count the members of.
/// @return The amount of members in `collection`.
size_t count_collection(void** collection);

/// @brief Copies the `source_collection`. Relies on a null-terminated member at the end of the collection.
/// @param source_collection The collection to copy.
/// @param callback A void pointer returning function that will run when copying the contents of the `source_collection`.
/// @return The copy of `source_collection`, or `NULL` if copying is unsuccessful.
void** copy_collection(void** source_collection, copy_function callback);

/// @brief Copies the `source_strings`. Syntactical sugar for `copy_collection` with a duplicate_string callback.
/// @param source_strings The strings to copy.
/// @return A copy of `source_strings`, or `NULL` if copying is unsuccessful.
char** copy_strings(char** source_strings);

#endif