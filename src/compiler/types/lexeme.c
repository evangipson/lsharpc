#include "lexeme.h"

/* ensures `new_lexeme->tokens` has no references to deallocated memory */
static bool copy_tokens(lexeme* new_lexeme, char** tokens)
{
    /* handle lexemes without tokens */
    if (tokens == NULL)
    {
        return false;
    }

    new_lexeme->tokens = copy_strings(tokens);
    if (new_lexeme->tokens == NULL)
    {
        log_error("[copy_expressions]: failed to copy tokens for lexeme");
        return false;
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

    new_lexeme->expressions = copy_strings(expressions);
    if (new_lexeme->expressions == NULL)
    {
        log_error("[copy_expressions]: failed to copy expressions for lexeme");
        return false;
    }
    return true;
}

lexeme* create_lexeme(char** tokens, char** expressions, size_t line_number)
{
    /* handle allocation failing */
    lexeme* new_lexeme = (lexeme*)safe_malloc(sizeof(lexeme));
    if (new_lexeme == NULL)
    {
        log_error("[create_lexeme]: failed to allocate lexeme");
        return NULL;
    }

    new_lexeme->line_number = line_number;

    /* if the lexeme has no expressions and no tokens, it was not allocated correctly, so return NULL */
    if (copy_tokens(new_lexeme, tokens) == false && copy_expressions(new_lexeme, expressions) == false)
    {
        log_error("[create_lexeme]: failed to create lexeme, no tokens or expressions provided");
        free_lexeme(new_lexeme);
        return NULL;
    }

    return new_lexeme;
}

void free_lexeme(lexeme* lexeme)
{
    if (lexeme == NULL)
    {
        log_error("[free_lexeme]: attempted to free a null lexeme");
        return;
    }

    safe_free_collection((void**)(lexeme->tokens));
    safe_free_collection((void**)(lexeme->expressions));
    safe_free((void*)lexeme);
}