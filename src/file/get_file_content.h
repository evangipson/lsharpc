#ifndef GET_FILE_CONTENT
#define GET_FILE_CONTENT
#include <assert.h>
#include <stdio.h>
#include "../log/logger.h"
#endif

/// @brief Gets all binary content from a file, and returns it.
/// @param file_name Thee path to an L# file.
/// @returns The file contents.
const char* get_file_content(const char* file_name);