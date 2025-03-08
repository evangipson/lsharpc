#ifndef BYTECODE_GENERATOR
#define BYTECODE_GENERATOR
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../debug/logger.h"
#include "../extensions/memory_extensions.h"
#include "../types/abstract_syntax_tree.h"
#include "../types/symbol_table.h"

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

/// @struct bytecode_generator
/// @brief The bytecode generator that compiles the abstract syntax tree into L# instructions.
typedef struct bytecode_generator bytecode_generator;

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

struct bytecode_generator
{
    instruction* instructions;
    int instruction_count;
    int instruction_capacity;
    symbol_table* symbols;
    int variable_count;
    int function_count;
};

/// @brief Creates a `bytecode_generator`.
/// @return A new bytecode generator.
bytecode_generator* create_bytecode_generator();

/// @brief Adds the `new_instruction` to the `generator`.
/// @param generator The `bytecode_generator` to add an instruction to.
/// @param instruction The `new_instruction` to add to the bytecode generator.
void emit_instruction(bytecode_generator* generator, instruction new_instruction);

/// @brief Generates bytecode for the abstract syntax `node`.
/// @param generator The `bytecode_generator` to add the generated bytecode to.
/// @param node The abstract syntax node that will have bytecode generated for it.
void generate_bytecode(bytecode_generator* generator, abstract_syntax_node* node);

/// @brief Compiles an abstract syntax tree into bytecode.
/// @param ast The abstract syntax tree to compile into bytecode.
/// @param instruction_count The amount of instructions.
/// @param output_filename The name of the file the bytecode will be written to.
/// @return A collection of `instruction` in bytecode.
instruction* compile_ast_to_bytecode(abstract_syntax_node* ast, int* instruction_count, const char* output_filename);

/// @brief Writes the bytecode `instructions` to the `filename` provided.
/// @param instructions A collection of bytecode instructions to write to a file.
/// @param instruction_count The number of bytecode instructions that will be written.
/// @param filename The name of the file to write the bytecode instructions to.
void write_bytecode_to_file(instruction* instructions, int instruction_count, const char* filename);

/// @brief Deallocates the memory used for the `generator`.
/// @param generator The `bytecode_generator` to free.
void free_bytecode_generator(bytecode_generator* generator);

#endif