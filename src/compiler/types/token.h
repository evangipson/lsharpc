#ifndef TOKEN
#define TOKEN
#include <stdbool.h>
#include <stdlib.h>
#include "../debug/logger.h"
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"

/// @enum token_type
/// @brief A collection of L# token types.
typedef enum token_type {
    TOKEN_KEYWORD_GRAB,
    TOKEN_KEYWORD_CONTRACT,
    TOKEN_KEYWORD_IS,
    TOKEN_KEYWORD_RETURN,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_TEXT,
    TOKEN_TYPE_BIT,
    TOKEN_TYPE_NUMBERS,
    TOKEN_TYPE_TEXTS,
    TOKEN_TYPE_BITS,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER_LITERAL,
    TOKEN_TEXT_LITERAL,
    TOKEN_BIT_LITERAL_ON,
    TOKEN_BIT_LITERAL_OFF,
    TOKEN_OPERATOR_ASSIGN,
    TOKEN_OPERATOR_PLUS,
    TOKEN_OPERATOR_MINUS,
    TOKEN_OPERATOR_MULTIPLY,
    TOKEN_OPERATOR_DIVIDE,
    TOKEN_PUNCTUATION_PERIOD,
    TOKEN_PUNCTUATION_COMMA,
    TOKEN_PUNCTUATION_OPEN_PAREN,
    TOKEN_PUNCTUATION_CLOSE_PAREN,
    TOKEN_PUNCTUATION_OPEN_BRACKET,
    TOKEN_PUNCTUATION_CLOSE_BRACKET,
    TOKEN_STRING_LITERAL,
    TOKEN_STRING_TEMPLATE_START,
    TOKEN_STRING_TEMPLATE_END,
    TOKEN_END_OF_FILE,
    TOKEN_ERROR
} token_type;

typedef struct token token;

struct token
{
    token_type type;
    /* can be NULL for tokens without values (e.g.: operators) */
    char* value;
    int line;
    int column;
};

/// @brief Gets the human-readable name of the `type`.
/// @param type The `token_type` to get the name of.
/// @return The name of the `type`.
const char* get_token_type_name(token_type type);

/// @brief Creates a token.
/// @param type The type of token to create.
/// @param value The value of the token. Can be NULL for tokens without values (e.g.: operators).
/// @param line The line number that this token originates from in the L# source code.
/// @param column The column number that this token originates from in the L# source code.
/// @return A new token, or `NULL` if creation was unsuccessful.
token* create_token(token_type type, char* value, int line, int column);

/// @brief Frees the memory allocated to the `token`.
/// @param token The token to free memory for.
/// @return `true` if deallocation was successful, `false` otherwise.
bool free_token(token* token);

/// @brief Frees the memory allocated to a collection of `tokens`.
/// @param tokens The token collection to free memory for.
/// @return `true` if deallocation was successful, `false` otherwise.
bool free_tokens(token** tokens);

#endif