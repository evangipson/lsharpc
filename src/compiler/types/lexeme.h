#ifndef LEXEME
#define LEXEME
#include <stdbool.h>
#include <stdlib.h>
#include "../extensions/collection_extensions.h"
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"

/// @struct lexeme
/// @brief One or many words that will be compiled into a `statement`.
typedef struct lexeme lexeme;

struct lexeme
{
    char** tokens;
    char** expressions;
    unsigned int line_number;
};

/// @brief Creates and returns a new `lexeme`. If tokens and expressions are both `NULL`, will return `NULL`.
/// @param tokens An optional collection of keywords, variables, or operators.
/// @param expressions An optional collection of expressions which aren't transformed before being invoked.
/// @param line_number The line number the `lexeme` was written on in the L# source code.
lexeme* create_lexeme(char** tokens, char** expressions, unsigned int line_number);

/// @brief Deallocates the memory of the `lexeme`.
void free_lexeme(lexeme* lexeme);

#endif