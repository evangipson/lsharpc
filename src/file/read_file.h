#ifndef READ_FILE
#define READ_FILE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#endif

/// @brief Gets all binary content from the file at `file_path`.
/// @param file_path The path to an L# file.
/// @return The content of the file.
char* read_file(const char* file_path);