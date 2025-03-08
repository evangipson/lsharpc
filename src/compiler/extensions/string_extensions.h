#ifndef STRING_EXTENSIONS
#define STRING_EXTENSIONS
#include <stdlib.h>
#include <string.h>
#include "../debug/logger.h"
#include "memory_extensions.h"

/// @brief Duplicates the `source`, and returns the result.
/// @return The duplicated `source`, or `NULL` if the duplication fails.
char* duplicate_string(const char* source);

#endif