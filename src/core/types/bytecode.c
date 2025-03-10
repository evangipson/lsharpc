#include "bytecode.h"

const char* get_op_code_name(op_code opcode)
{
    switch (opcode)
    {
        case OP_LOAD_CONST: return "OP_LOAD_CONST";
        case OP_LOAD_VAR: return "OP_LOAD_VAR";
        case OP_STORE_VAR: return "OP_STORE_VAR";
        case OP_POP: return "OP_POP";
        case OP_ADD: return "OP_ADD";
        case OP_SUB: return "OP_SUB";
        case OP_MUL: return "OP_MUL";
        case OP_DIV: return "OP_DIV";
        case OP_EQ: return "OP_EQ";
        case OP_NEQ: return "OP_NEQ";
        case OP_GT: return "OP_GT";
        case OP_LT: return "OP_LT";
        case OP_AND: return "OP_AND";
        case OP_OR: return "OP_OR";
        case OP_NOT: return "OP_NOT";
        case OP_JMP: return "OP_JMP";
        case OP_JMP_IF_FALSE: return "OP_JMP_IF_FALSE";
        case OP_CALL: return "OP_CALL";
        case OP_RETURN: return "OP_RETURN";
        case OP_HALT: return "OP_HALT";
        case OP_PRINT: return "OP_PRINT";
        default: return "UNKNOWN OP CODE";
    }
}

const char* get_op_type_name(op_type optype)
{
    switch (optype)
    {
        case OP_TYPE_NULL: return "OP_TYPE_NULL";
        case OP_TYPE_BIT: return "OP_TYPE_BIT";
        case OP_TYPE_INDEX: return "OP_TYPE_INDEX";
        case OP_TYPE_NUMBER: return "OP_TYPE_NUMBER";
        case OP_TYPE_TEXT: return "OP_TYPE_TEXT";
        case OP_TYPE_VARIABLE: return "OP_TYPE_VARIABLE";
        default: return "UNKNOWN OP TYPE";
    }
}