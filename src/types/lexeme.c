#include "lexeme.h"

lexeme create_lexeme(char** tokens, char** expressions, unsigned int line_number)
{
    lexeme new_lexeme = { tokens, expressions, line_number };
    return new_lexeme;
}