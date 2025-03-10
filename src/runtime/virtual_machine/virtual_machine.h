#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../core/extensions/memory_extensions.h"
#include "../../core/logger/logger.h"
#include "../../core/types/bytecode.h"

/// @struct virtual_machine
/// @brief The virtual machine that runs L# instructions, and manages the stack.
typedef struct virtual_machine virtual_machine;

struct virtual_machine
{
    value* stack;
    int stack_size;
    int stack_pointer;
    instruction* instructions;
    int instruction_count;
    int instruction_capacity;
    value* variables;
    int variable_count;
    /// @brief An array of pointers to managed objects.
    char** objects;
    int object_count;
    int object_capacity;
    /// @brief The flags for garbage collection.
    char* object_flags;
    int program_counter;
};

/// @brief Creates a `virtual_machine` and fills it with default data.
/// @return `true` if the virtual machine was created successfully, `false` otherwise.
bool create_virtual_machine(virtual_machine* vm);

/// @brief Loads bytecode from a file.
/// @param vm The virtual machine to load the bytecode into.
/// @param filename The name of the bytecode file.
/// @return `true` if the bytecode file is loaded, `false` otherwise.
bool load_bytecode_from_file(virtual_machine* vm, const char* filename);

/// @brief Runs the `vm`.
/// @param vm The virtual machine to run.
/// @return `true` if the virtual machine was run, `false` otherwise.
bool run_vm(virtual_machine* vm);

/// @brief Marks and sweep objects that can be cleaned.
/// @param virtual_machine The L# virtual machine.
void garbage_collect(virtual_machine* virtual_machine);

#endif