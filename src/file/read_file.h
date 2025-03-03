#ifndef READ_FILE
#define READ_FILE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#endif

/// @brief Gets all binary content from a file and allocates it to `file_contents`.
/// @param file_name The path to an L# file.
/// @param file_contents A pointer to a string which will be filled with file contents.
void allocate_file_to_buffer(const char* file_name, char** file_contents);