#ifndef STATEMENT
#define STATEMENT
#include <stdbool.h>
#include <stdlib.h>
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"
#include "lsharp_state.h"

/// @struct statement
/// @brief Filled with compiler information and an `expression` that will be executed by the L# compiler.
typedef struct statement statement;

struct statement
{
    unsigned int line_number;
    char* file_path;
    char* type;
    char* action;
    char* name;
    bool (*expression)();
};

/// @brief Creates and returns a new `statement`.
/// @param line_number The line number the `statement` started on in the L# source code.
/// @param file_path The L# source file that this `statement` originates from.
/// @param type The type of the new `statement`.
/// @param action The action of the new `statement`.
/// @param name The name of the new `statement`.
/// @param expression The `function` that defines the behavior of the new `statement`.
statement* create_statement(unsigned int line_number, char* file_path, char* type, char* action, char* name, bool (*expression)());

/// @brief Deallocates the memory of the `statement`.
void free_statement(statement* statement);

#endif