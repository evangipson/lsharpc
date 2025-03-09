#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE
#include <stdint.h>
#include <stdlib.h>

/// @enum op_code
/// @brief A collection of operation codes for the L# virtual machine.
typedef enum op_code {
    OP_LOAD_CONST,
    OP_LOAD_VAR,
    OP_STORE_VAR,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_NEQ,
    OP_GT,
    OP_LT,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_CALL,
    OP_RETURN,
    OP_HALT,
    OP_PRINT,
} op_code;

/// @enum value_type
/// @brief Contains various L# value representations.
typedef enum value_type {
    VAL_INT,
    VAL_DOUBLE,
    VAL_STRING,
    VAL_BOOL,
    VAL_NULL
} value_type;

/// @struct value
/// @brief A value in the L# virtual machine.
typedef struct value value;

/// @struct instruction
/// @brief An instruction structure for L# bytecode.
typedef struct instruction instruction;

/// @struct virtual_machine
/// @brief The virtual machine that runs L# instructions, and manages the stack.
typedef struct virtual_machine virtual_machine;

struct value
{
    value_type type;
    union
    {
        int32_t i;
        double d;
        char* s;
        uint8_t b;
    } as;
};

struct instruction
{
    op_code op_code;
    union
    {
        /// @brief integer operand
        int32_t i;
        /// @brief double operand
        double d;
        /// @brief string operand
        char* s;
        /// @brief boolean operand
        uint8_t b;

        struct
        {
            int32_t jump_offset;
        } jump;

        struct
        {
            int32_t function_index;
        } call;

        struct
        {
            int32_t variable_index;
        } variable;
    } operand;
};

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
    void** objects;
    int object_count;
    int object_capacity;
    /// @brief The flags for garbage collection.
    char* object_flags;
    int program_counter;
};

/// @brief Marks and sweep objects that can be cleaned.
/// @param virtual_machine The L# virtual machine.
void garbage_collect(virtual_machine* virtual_machine);

#endif