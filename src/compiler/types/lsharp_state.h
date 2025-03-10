#ifndef LSHARP_STATE
#define LSHARP_STATE
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../core/extensions/collection_extensions.h"
#include "../../core/extensions/memory_extensions.h"
#include "../../core/extensions/string_extensions.h"

/// @enum error_type
/// @brief A collection of compiler error types.
typedef enum error_type
{
    ERROR_LEXICAL,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
} error_type;

/// @struct variable
/// @brief An allocated object during program execution.
typedef struct variable variable;

/// @struct stackframe
/// @brief A collection of callers and their results during program execution.
typedef struct stackframe stackframe;

/// @struct compiler_error
/// @brief Error state information from the compiler during program compilation.
typedef struct compiler_error compiler_error;

/// @struct lsharp_state
/// @brief A stateful context that is a series of snapshots of information for an L# program.
typedef struct lsharp_state lsharp_state;

struct variable
{
    char* type;
    void* value;
    char* name;
    struct variable* next;
};

struct stackframe
{
    char* caller_name;
    int line_number;
    char* file_path;
    variable* variables;
    struct stackframe* previous;
};

struct compiler_error
{
    char* file_path;
    int line_number;
    int column;
    char* message;
    error_type type;
    struct compiler_error* next;
};

struct lsharp_state
{
    variable* variables;
    stackframe* stack;
    compiler_error* errors;
};

/// @brief Creates an L# program state.
lsharp_state* create_lsharp_state();

/// @brief Resets the `state` by freeing all memory associated with all members of `state`.
/// @param state The L# program state to reset.
void reset_lsharp_state(lsharp_state* state);

/// @brief Adds a variable to the current stack of an L# program, returns `true` if it was successfully added and `false` otherwise.
/// @param state The L# program state to add a variable to.
/// @param variable_type The type of variable to add.
/// @param variable_name The name of the variable to add.
/// @param variable_value The value of the variable to add.
bool add_variable(lsharp_state* state, char* variable_type, char* variable_name, void* variable_value);

/// @brief Gets a variable from the L# `state`.
/// @param state The L# program state to get a variable from.
/// @param variable_name The name of the variable to get.
variable* get_variable(lsharp_state* state, char* variable_name);

/// @brief Adds a stack frame to the L# `state`, returns `true` if it was successfully added and `false` otherwise.
/// @param state The L# program state to add a stackframe to.
/// @param caller_name The name of the calling method that is adding a stack frame.
/// @param line_number The line number of the L# source code file that is adding a stack frame.
/// @param file_path Path to the L# source code file that is adding a stack frame.
bool add_stackframe(lsharp_state* state, char* caller_name, int line_number, char* file_path);

/// @brief Gets the current stackframe's file path and line number, separated by a colon.
/// @param state The L# program state to get the current file path and line number from.
char* get_current_frame_file_and_line(lsharp_state* state);

/// @brief Adds a compiler error to the L# `state`.
/// @param state The L# program state to add the compiler error to.
/// @param file_path Path to the L# source code file that is adding an error.
/// @param line_number The line number of the L# source code file that is adding an error.
/// @param column The column of the L# source code file that is adding an error.
/// @param message The error message to add to the L# program state.
/// @param type The type of error to add to the L# program state.
void add_compiler_error(lsharp_state* state, char* file_path, int line_number, int column, char* message, error_type type);

/// @brief Prints all of the current `state` compiler errors.
/// @param state The L# program state to report compiler errors from.
void report_compiler_error(lsharp_state* state);

#endif