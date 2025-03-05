#include "abstract_syntax_tree.h"

abstract_syntax_node* create_ast_node_variable(char* name)
{
    abstract_syntax_node* node = (abstract_syntax_node*)safe_malloc(sizeof(abstract_syntax_node));
    if (node == NULL)
    {
        return NULL;
    }

    node->type = AST_NODE_VARIABLE;
    node->data.variable.name = duplicate_string(name);
    node->next = NULL;

    if(node->data.variable.name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_binary_op(abstract_syntax_node* left, abstract_syntax_node* right, char* comparator)
{
    abstract_syntax_node* node = (abstract_syntax_node*)safe_malloc(sizeof(abstract_syntax_node));
    if (node == NULL)
    {
        return NULL;
    }

    node->type = AST_NODE_BINARY_OP;
    node->data.binary_operation.left = left;
    node->data.binary_operation.right = right;
    node->data.binary_operation.comparator = duplicate_string(comparator);
    node->next = NULL;

    if(node->data.binary_operation.comparator == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_function_call(char* function_name, abstract_syntax_node** arguments, int argument_count)
{
    abstract_syntax_node* node = (abstract_syntax_node*)safe_malloc(sizeof(abstract_syntax_node));
    if (node == NULL)
    {
        return NULL;
    }

    node->type = AST_NODE_FUNCTION_CALL;
    node->data.function_call.function_name = duplicate_string(function_name);
    node->data.function_call.arguments = arguments;
    node->data.function_call.argument_count = argument_count;
    node->next = NULL;

    if(node->data.function_call.function_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

void free_ast_node(abstract_syntax_node* node)
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
        case AST_NODE_VARIABLE:
            safe_free(node->data.variable.name);
            break;
        case AST_NODE_BINARY_OP:
            free_ast_node(node->data.binary_operation.left);
            free_ast_node(node->data.binary_operation.right);
            safe_free(node->data.binary_operation.comparator);
            break;
        case AST_NODE_FUNCTION_CALL:
            safe_free(node->data.function_call.function_name);
            break;
    }

    safe_free(node);
}