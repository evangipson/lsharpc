#ifndef STATEMENT
#define STATEMENT

#include <stdbool.h>
#include <stdlib.h>

/// @struct statement
/// @brief Filled with compiler information and an `expression` that will be executed by the L# compiler.
typedef struct statement_struct
{
    /// @brief The line number the `statement` started on in the L# source code.
    unsigned int line_number;
    /// @brief The L# source file that this `statement` originates from.
    char* file_path;
    /// @brief The type of a `statement`.
    char* type;
    /// @brief The action of a `statement`.
    char* action;
    /// @brief The name of a `statement`, for internal compiler use.
    char* name;
    /// @brief The invokable behavior of a `statement`.
    bool (*expression)();
} statement;

/// @brief Creates and returns a new `statement`.
/// @param line_number The line number the `statement` started on in the L# source code.
/// @param file_path The L# source file that this `statement` originates from.
/// @param type The type of the new `statement`.
/// @param action The action of the new `statement`.
/// @param name The name of the new `statement`.
/// @param expression The `function` that defines the behavior of the new `statement`.
statement create_statement(unsigned int line_number, char* file_path, char* type, char* action, char* name, bool (*expression)());

#endif