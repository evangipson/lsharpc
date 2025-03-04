#ifndef GET_LEXEMES
#define GET_LEXEMES
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../debug/logger.h"
#include "../types/lexeme.h"

/// @brief Gets a collection of lexemes from `file_content`.
/// @param file_content The content of the file to parse.
lexeme** get_lexemes(char* file_content);

#endif