#ifndef BYTECODE
#define BYTECODE
#include <stdint.h>
#include "symbol_table.h"

/// @enum op_code
/// @brief A collection of operation codes for the L# virtual machine.
typedef enum op_code
{
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
typedef enum value_type
{
    VAL_INT,
    VAL_DOUBLE,
    VAL_STRING,
    VAL_BOOL,
    VAL_NULL
} value_type;

/// @enum op_type
/// @brief Tells the LVM what the instruction contains.
typedef enum op_type
{
    OP_TYPE_NULL,
    OP_TYPE_INDEX,
    OP_TYPE_NUMBER,
    OP_TYPE_BIT,
    OP_TYPE_TEXT,
    OP_TYPE_VARIABLE
} op_type;

/// @struct value
/// @brief A value in the L# virtual machine.
typedef struct value value;

/// @struct instruction
/// @brief An instruction structure for L# bytecode.
typedef struct instruction instruction;

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
    /// @brief The operation code to be executed by the LVM.
    op_code op_code;

    /// @brief The value associated with the operation.
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
            symbol_type variable_type;
        } variable;
    } operand;

    /// @brief The type of operand for this instruction.
    op_type op_type;
};

/// @brief Gets a human-readable name of the `opcode`.
/// @param opcode The `op_code` to get a human-readable name for.
/// @return The name of the `opcode`.
const char* get_op_code_name(op_code opcode);

/// @brief Gets a human-readable name of the `optype`.
/// @param optype The `op_type` to get a human-readable name for.
/// @return The name of the `optype`.
const char* get_op_type_name(op_type optype);

#endif