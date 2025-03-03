#ifndef LEXEME
#define LEXEME

/// @struct lexeme
/// @brief One or many words that will be compiled into a `statement`.
typedef struct lexeme_struct
{
    /// @brief A collection of keywords, variables, or operators.
    char** tokens;
    /// @brief A collection of expressions which aren't transformed before being invoked.
    char** expressions;
    /// @brief The line number the `lexeme` was written on in the L# source code.
    unsigned int line_number;
} lexeme;

/// @brief Creates and returns a new `lexeme`.
/// @param tokens A collection of keywords, variables, or operators.
/// @param expressions A collection of expressions which aren't transformed before being invoked.
/// @param line_number The line number the `lexeme` was written on in the L# source code.
lexeme create_lexeme(char** tokens, char** expressions, unsigned int line_number);

#endif