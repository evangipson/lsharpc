#ifndef LEXEME
#endif

typedef struct
{
    char** tokens;
    char** expressions;
    unsigned int line_number;
} LEXEME;

LEXEME create_lexeme(char** tokens, char** expressions, unsigned int line_number);