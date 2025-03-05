#ifndef ABSTRACT_SYNTAX_TREE
#define ABSTRACT_SYNTAX_TREE
#include "../extensions/collection_extensions.h"
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"

typedef enum
{
    AST_NODE_VARIABLE,
    AST_NODE_BINARY_OP,
    AST_NODE_FUNCTION_CALL,
} abstract_syntax_node_type;

typedef struct abstract_syntax_node abstract_syntax_node;

struct abstract_syntax_node
{
    abstract_syntax_node_type type;
    struct abstract_syntax_node* next;

    union
    {
        struct
        {
            char* name;
        } variable;

        struct
        {
            struct abstract_syntax_node* left;
            struct abstract_syntax_node* right;
            char* comparator;
        } binary_operation;

        struct
        {
            char* function_name;
            struct abstract_syntax_node** arguments;
            int argument_count;
        } function_call;
    } data;
};

abstract_syntax_node* create_ast_node_variable(char* name);

abstract_syntax_node* create_ast_node_binary_op(abstract_syntax_node* left, abstract_syntax_node* right, char* comparator);

abstract_syntax_node* create_ast_node_function_call(char* function_name, abstract_syntax_node** arguments, int argument_count);

#endif