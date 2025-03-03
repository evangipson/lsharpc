#ifndef GET_LEXEMES
#define GET_LEXEMES
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../log/logger.h"
#endif

/// @brief Gets a collection of lexemes from `file_content`.
/// @param file_content The content of the file to parse.
char** get_lexemes(char* file_content);