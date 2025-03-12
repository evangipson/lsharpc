#include "token.h"

const char* get_token_type_name(token_type type)
{
   switch (type)
   {
        case TOKEN_KEYWORD_GRAB: return "TOKEN_KEYWORD_GRAB";
        case TOKEN_KEYWORD_CONTRACT: return "TOKEN_KEYWORD_CONTRACT";
        case TOKEN_KEYWORD_IS: return "TOKEN_KEYWORD_IS";
        case TOKEN_KEYWORD_RETURN: return "TOKEN_KEYWORD_RETURN";
        case TOKEN_TYPE_NUMBER: return "TOKEN_TYPE_NUMBER";
        case TOKEN_TYPE_TEXT: return "TOKEN_TYPE_TEXT";
        case TOKEN_TYPE_BIT: return "TOKEN_TYPE_BIT";
        case TOKEN_TYPE_NUMBERS: return "TOKEN_TYPE_NUMBERS";
        case TOKEN_TYPE_TEXTS: return "TOKEN_TYPE_TEXTS";
        case TOKEN_TYPE_BITS: return "TOKEN_TYPE_BITS";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER_LITERAL: return "TOKEN_NUMBER_LITERAL";
        case TOKEN_TEXT_LITERAL: return "TOKEN_TEXT_LITERAL";
        case TOKEN_BIT_LITERAL_ON: return "TOKEN_BIT_LITERAL_ON";
        case TOKEN_BIT_LITERAL_OFF: return "TOKEN_BIT_LITERAL_OFF";
        case TOKEN_OPERATOR_ASSIGN: return "TOKEN_OPERATOR_ASSIGN";
        case TOKEN_OPERATOR_PLUS: return "TOKEN_OPERATOR_PLUS";
        case TOKEN_OPERATOR_MINUS: return "TOKEN_OPERATOR_MINUS";
        case TOKEN_OPERATOR_MULTIPLY: return "TOKEN_OPERATOR_MULTIPLY";
        case TOKEN_OPERATOR_DIVIDE: return "TOKEN_OPERATOR_DIVIDE";
        case TOKEN_PUNCTUATION_PERIOD: return "TOKEN_PUNCTUATION_PERIOD";
        case TOKEN_PUNCTUATION_COMMA: return "TOKEN_PUNCTUATION_COMMA";
        case TOKEN_PUNCTUATION_OPEN_PAREN: return "TOKEN_PUNCTUATION_OPEN_PAREN";
        case TOKEN_PUNCTUATION_CLOSE_PAREN: return "TOKEN_PUNCTUATION_CLOSE_PAREN";
        case TOKEN_PUNCTUATION_OPEN_BRACKET: return "TOKEN_PUNCTUATION_OPEN_BRACKET";
        case TOKEN_PUNCTUATION_CLOSE_BRACKET: return "TOKEN_PUNCTUATION_CLOSE_BRACKET";
        case TOKEN_STRING_LITERAL: return "TOKEN_STRING_LITERAL";
        case TOKEN_STRING_TEMPLATE_START: return "TOKEN_STRING_TEMPLATE_START";
        case TOKEN_STRING_TEMPLATE_END: return "TOKEN_STRING_TEMPLATE_END";
        case TOKEN_END_OF_FILE: return "TOKEN_END_OF_FILE";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        default: return "TOKEN_UNKNOWN";
   }
}

token* create_token(token_type type, char* value, int line, int column)
{
    /* handle allocation failing */
    token* new_token = (token*)safe_malloc(sizeof(token));
    if (new_token == NULL)
    {
        return NULL;
    }

    new_token->type = type;
    new_token->value = duplicate_string(value);
    new_token->line = line;
    new_token->column = column;

    /* if any expected properties haven't been properly allocated, free the memory and return NULL */
    if (new_token->value == NULL && value != NULL)
    {
        log_error("Compiler error: Failed to create token, value was not correctly set.");
        free_token(new_token);
        return NULL;
    }

    return new_token;
}

bool free_token(token* token)
{
    if (token == NULL)
    {
        log_error("Compiler error: Attempted to free an already null token.");
        return false;
    }

    safe_free((void*)token->value);
    safe_free((void*)token);
    return true;
}

bool free_tokens(token** tokens)
{
    if (tokens == NULL)
    {
        log_error("Compiler error: Attempted to free an already null collection of tokens.");
        return false;
    }

    bool is_token_free = false;
    for (size_t i = 0; tokens[i] != NULL; i++)
    {
        is_token_free = free_token(tokens[i]);
        if (is_token_free == false)
        {
            safe_free((void*)tokens);
            return false;
        }
    }

    safe_free((void*)tokens);
    return true;
}