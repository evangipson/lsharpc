#include "main.h"

static void print_tokens(token** tokens)
{
    if (tokens == NULL)
    {
        log_info("[lsharpc]: failed to get tokens from L# source file");
        return;
    }

    log_info("[lsharpc]: got tokens from L# source file");
    for (size_t i = 0; tokens[i] != NULL; i++)
    {
        printf("\ttoken %zu (line %d, column %d)\n", i, tokens[i]->line, tokens[i]->column);
        printf("\t  type  : %s\n", get_token_type_name(tokens[i]->type));
        printf("\t  value : %s\n", tokens[i]->value);
    }
}

const char* get_ast_node_type_name(abstract_syntax_node_type type)
{
    switch (type)
    {
        case AST_NODE_PROGRAM: return "PROGRAM";
        case AST_NODE_STATEMENT: return "STATEMENT";
        case AST_NODE_DECLARATION: return "DECLARATION";
        case AST_NODE_ASSIGNMENT: return "ASSIGNMENT";
        case AST_NODE_FUNCTION_CALL: return "FUNCTION CALL";
        case AST_NODE_CONTRACT_DEFINITION: return "CONTRACT DEFINITION";
        case AST_NODE_RETURN_STATEMENT: return "RETURN_STATEMENT";
        case AST_NODE_EXPRESSION: return "EXPRESSION";
        case AST_NODE_TERM: return "TERM";
        case AST_NODE_FACTOR: return "FACTOR";
        case AST_NODE_LITERAL: return "LITERAL";
        case AST_NODE_ARRAY_LITERAL: return "ARRAY LITERAL";
        case AST_NODE_TYPE: return "TYPE";
        case AST_NODE_NUMBER_LITERAL: return "NUMBER_LITERAL";
        case AST_NODE_STRING_LITERAL: return "STRING_LITERAL";
        case AST_NODE_BIT_LITERAL: return "BIT LITERAL";
        case AST_NODE_IDENTIFIER: return "IDENTIFIER";
        case AST_NODE_BINARY_OP: return "BINARY_OPERATION";
        case AST_NODE_TEMPLATE_STRING: return "TEMPLATE STRING";
        case AST_NODE_GRAB_STATEMENT: return "GRAB STATEMENT";
        case AST_NODE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

static void print_ast_node(abstract_syntax_node* node, int indent) {
    if (node == NULL)
    {
        return;
    }

    /* print indentation */
    for (int i = 0; i < indent; i++)
    {
        printf("  ");
    }

    /* print node information */
    printf("Node: %s", get_ast_node_type_name(node->type));

    /* print node-specific data */
    switch (node->type)
    {
        case AST_NODE_PROGRAM:
            printf("\n");
            for(int i = 0; i < node->data.program_node.statement_count; i++)
            {
                print_ast_node(node->data.program_node.statements[i], indent + 1);
            }
            break;
        case AST_NODE_STATEMENT:
            printf("\n");
            print_ast_node(node->data.statement_node.statement, indent + 1);
            break;
        case AST_NODE_DECLARATION:
            printf(", Type: %s, Variable Name: %s\n", node->data.declaration_node.type_name, node->data.declaration_node.variable_name);
            print_ast_node(node->data.declaration_node.expression, indent + 1);
            break;
        case AST_NODE_ASSIGNMENT:
            printf(", Variable Name: %s\n", node->data.assignment.variable_name);
            print_ast_node(node->data.assignment.expression, indent + 1);
            break;
        case AST_NODE_FUNCTION_CALL:
            printf(", Function Name: %s\n", node->data.function_call.function_name);
            for(int i = 0; i < node->data.function_call.argument_count; i++)
            {
                print_ast_node(node->data.function_call.arguments[i], indent + 1);
            }
            break;
        case AST_NODE_CONTRACT_DEFINITION:
            printf(", Name: %s\n", node->data.contract_definition_node.contract_name);
            for(int i = 0; i < node->data.contract_definition_node.declaration_count; i++)
            {
                print_ast_node(node->data.contract_definition_node.declarations[i], indent + 1);
            }
            break;
        case AST_NODE_RETURN_STATEMENT:
            printf("\n");
            print_ast_node(node->data.return_statement_node.expression, indent + 1);
            break;
        case AST_NODE_TERM:
        case AST_NODE_FACTOR:
        case AST_NODE_EXPRESSION:
            printf("\n");
            print_ast_node(node->left, indent + 1);
            printf("\n");
            print_ast_node(node->right, indent + 1);
            break;
        case AST_NODE_ARRAY_LITERAL:
            printf("\n");
            for(int i = 0; i < node->data.array_literal.element_count; i++)
            {
                print_ast_node(node->data.array_literal.elements[i], indent + 1);
            }
            break;
        case AST_NODE_TYPE:
            printf(", Type Name: %s\n", node->data.type_node.type_name);
            break;
        case AST_NODE_NUMBER_LITERAL:
            printf(", Value: %f\n", node->data.number_literal.value);
            break;
        case AST_NODE_STRING_LITERAL:
            printf(", Value: %s\n", node->data.string_literal.value);
            break;
        case AST_NODE_BIT_LITERAL:
            printf(", Value: %d\n", node->data.bit_literal.value);
            break;
        case AST_NODE_IDENTIFIER:
            printf(", Identifier: %s\n", node->data.identifier_node.identifier_name);
            break;
        case AST_NODE_BINARY_OP:
            printf(", Operator: %s\n", node->data.binary_operation.op_symbol);
            print_ast_node(node->left, indent + 1);
            print_ast_node(node->right, indent + 1);
            break;
        case AST_NODE_TEMPLATE_STRING:
            printf("\n");
            for(int i = 0; i < node->data.template_string_node.part_count; i++)
            {
                print_ast_node(node->data.template_string_node.parts[i], indent + 1);
            }
            break;
        case AST_NODE_GRAB_STATEMENT:
            printf("\n");
            print_ast_node(node->data.grab_statement_node.identifier, indent + 1);
            break;
        case AST_NODE_ERROR:
            printf(", Error: %s\n", node->data.error_node.message);
            break;
        default:
            printf("\n");
            break;
    }
}

static void print_ast(abstract_syntax_node* node) {
    if (node == NULL) {
        log_info("[lsharpc]: failed to get abstract syntax tree from tokens");
        return;
    }

    log_info("[lsharpc]: got abstract syntax tree from tokens");
    /* start printing from the top of the tree with no indentation */
    print_ast_node(node, 0);
}

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    log_info("[lsharpc]: attempting to read file \"%s\"", argv[1]);
    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    log_info("[lsharpc]: got content from L# source file \"%s\", running lexer", argv[1]);
    
    size_t token_count = 0;
    token** tokens = get_tokens(file_content, &token_count);
    print_tokens(tokens);

    abstract_syntax_node* abstract_syntax_tree = get_abstract_syntax_tree(tokens, token_count);
    print_ast(abstract_syntax_tree);

    free_tokens(tokens);
    free_ast(abstract_syntax_tree);

    return 1;
}