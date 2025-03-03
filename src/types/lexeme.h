#ifndef LEXEME
#define LEXEME

typedef struct
{
    char** tokens;
    char** expressions;
    unsigned int line_number;
} lexeme;

lexeme create_lexeme(char** tokens, char** expressions, unsigned int line_number);

#endif