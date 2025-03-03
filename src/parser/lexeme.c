#include "lexeme.h"

LEXEME create_lexeme(char** tokens, char** expressions, unsigned int line_number)
{
    LEXEME lexeme;
    lexeme.tokens = tokens;
    lexeme.expressions = expressions;
    lexeme.line_number = line_number;

    return lexeme;
}