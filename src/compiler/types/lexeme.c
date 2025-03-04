#include "lexeme.h"

/* ensures `new_lexeme->tokens` has no references to deallocated memory */
static bool copy_tokens(lexeme* new_lexeme, char** tokens)
{
    /* handle lexemes without tokens */
    if (tokens == NULL)
    {
        return false;
    }

    new_lexeme->tokens = (char**)copy_collection((void**)tokens);
    if (new_lexeme->tokens == NULL)
    {
        return false;
    }

    /* copy the strings that the array points to */
    size_t token_count = count_collection((void**)tokens);
    for(size_t i = 0; i < token_count; i++)
    {
        new_lexeme->tokens[i] = duplicate_string(tokens[i]);
        if(new_lexeme->tokens[i] == NULL)
        {
            safe_free_collection((void**)new_lexeme->tokens);
            return false;
        }
    }

    return true;
}

/* ensures `new_lexeme->expressions` has no references to deallocated memory */
static bool copy_expressions(lexeme* new_lexeme, char** expressions)
{
    /* handle lexemes without expressions */
    if (expressions == NULL)
    {
        return false;
    }

    new_lexeme->expressions = (char**)copy_collection((void**)expressions);
    if (new_lexeme->expressions == NULL)
    {
        return false;
    }

    /* copy the strings that the array points to */
    size_t expression_count = count_collection((void**)expressions);
    for(size_t i = 0; i < expression_count; i++)
    {
        new_lexeme->expressions[i] = duplicate_string(expressions[i]);
        if(new_lexeme->expressions[i] == NULL)
        {
            safe_free_collection((void**)new_lexeme->expressions);
            return false;
        }
    }

    return true;
}

lexeme* create_lexeme(char** tokens, char** expressions, unsigned int line_number)
{
    /* handle allocation failing */
    lexeme* new_lexeme = (lexeme*)safe_malloc(sizeof(lexeme));
    if (new_lexeme == NULL)
    {
        return NULL;
    }

    new_lexeme->line_number = line_number;

    /* if the lexeme has no expressions and no tokens, it was not allocated correctly, so return NULL */
    if (copy_tokens(new_lexeme, tokens) == false && copy_expressions(new_lexeme, expressions) == false)
    {
        free_lexeme(new_lexeme);
        return NULL;
    }

    return new_lexeme;
}

void free_lexeme(lexeme* lexeme)
{
    if (lexeme == NULL)
    {
        return;
    }

    safe_free_collection((void**)lexeme->tokens);
    safe_free_collection((void**)lexeme->expressions);
    safe_free(lexeme);
}