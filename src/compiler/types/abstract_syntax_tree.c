#include "abstract_syntax_tree.h"

static abstract_syntax_node* create_new_node(abstract_syntax_node_type type)
{
    abstract_syntax_node* node = (abstract_syntax_node*)safe_malloc(sizeof(abstract_syntax_node));
    if (node == NULL)
    {
        log_error("Compiler error: Unable to allocate memory for abstract syntax node.");
        return NULL;
    }

    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->children = NULL;

    return node;
}

void print_ast_node(abstract_syntax_node* node, int indent)
{
    /* guard against NULL nodes */
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
            printf(", Module Name: %s\n", node->data.grab_statement_node.module_name);
            break;
        case AST_NODE_ERROR:
            printf(", Error: %s\n", node->data.error_node.message);
            break;
        default:
            printf("\n");
            break;
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

abstract_syntax_node* create_ast_node_identifier(char* name)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_IDENTIFIER);

    node->data.identifier_node.identifier_name = duplicate_string(name);

    if(node->data.identifier_node.identifier_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_binary_operation(abstract_syntax_node* left, abstract_syntax_node* right, char* op_symbol)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_BINARY_OP);

    node->data.binary_operation.left = left;
    node->data.binary_operation.right = right;
    node->data.binary_operation.op_symbol = duplicate_string(op_symbol);

    if(node->data.binary_operation.op_symbol == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_function_call(char* function_name, abstract_syntax_node** arguments, int argument_count, int argument_capacity)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_FUNCTION_CALL);

    node->data.function_call.function_name = duplicate_string(function_name);
    node->data.function_call.arguments = arguments;
    node->data.function_call.argument_count = argument_count;
    node->data.function_call.argument_capacity = argument_capacity;

    if(node->data.function_call.function_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_assignment(char* variable_name, abstract_syntax_node* expression)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_ASSIGNMENT);

    node->data.assignment.variable_name = duplicate_string(variable_name);
    node->data.assignment.expression = expression;

    if(node->data.assignment.variable_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_number_literal(double value)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_NUMBER_LITERAL);

    node->data.number_literal.value = value;

    return node;
}

abstract_syntax_node* create_ast_node_string_literal(char* value)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_STRING_LITERAL);

    node->data.string_literal.value = duplicate_string(value);

    if(node->data.string_literal.value == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_bit_literal(bool value)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_BIT_LITERAL);

    node->data.bit_literal.value = value;

    return node;
}

abstract_syntax_node* create_ast_node_array_literal(abstract_syntax_node** elements, int element_count, int element_capacity)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_ARRAY_LITERAL);

    node->data.array_literal.elements = elements;
    node->data.array_literal.element_count = element_count;
    node->data.array_literal.element_capacity = element_capacity;

    return node;
}

abstract_syntax_node* create_ast_node_type(char* type_name)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_TYPE);

    node->data.type_node.type_name = duplicate_string(type_name);

    if(node->data.type_node.type_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_program(abstract_syntax_node** statements, int statement_count, int statement_capacity)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_PROGRAM);
    
    node->data.program_node.statements = statements;
    node->data.program_node.statement_count = statement_count;
    node->data.program_node.statement_capacity = statement_capacity;

    return node;
}

abstract_syntax_node* create_ast_node_statement(abstract_syntax_node* statement)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_STATEMENT);

    node->data.statement_node.statement = statement;

    return node;
}

abstract_syntax_node* create_ast_node_declaration(char* type_name, char* variable_name, struct abstract_syntax_node* expression)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_DECLARATION);

    node->data.declaration_node.type_name = duplicate_string(type_name);
    node->data.declaration_node.variable_name = duplicate_string(variable_name);
    node->data.declaration_node.expression = expression;

    if(node->data.declaration_node.type_name == NULL || node->data.declaration_node.variable_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_contract_definition(char* contract_name, abstract_syntax_node** declarations, int declaration_count, int declaration_capacity)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_CONTRACT_DEFINITION);

    node->data.contract_definition_node.contract_name = duplicate_string(contract_name);
    node->data.contract_definition_node.declarations = declarations;
    node->data.contract_definition_node.declaration_count = declaration_count;
    node->data.contract_definition_node.declaration_capacity = declaration_capacity;

    if (node->data.contract_definition_node.contract_name == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}

abstract_syntax_node* create_ast_node_return_statement(abstract_syntax_node* expression)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_RETURN_STATEMENT);

    node->data.return_statement_node.expression = expression;

    return node;
}

abstract_syntax_node* create_ast_node_template_string(abstract_syntax_node** parts, int part_count, int part_capacity)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_TEMPLATE_STRING);

    node->data.template_string_node.parts = parts;
    node->data.template_string_node.part_count = part_count;
    node->data.template_string_node.part_capacity = part_capacity;

    return node;
}

abstract_syntax_node* create_ast_node_grab_statement(char* module_name)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_GRAB_STATEMENT);

    node->data.grab_statement_node.module_name = module_name;

    return node;
}

abstract_syntax_node* create_ast_node_error(const char* message, int line, int column, int code)
{
    abstract_syntax_node* node = create_new_node(AST_NODE_ERROR);

    node->data.error_node.message = duplicate_string(message);
    node->data.error_node.line = line;
    node->data.error_node.column = column;
    node->data.error_node.code = code;

    if(node->data.error_node.message == NULL)
    {
        safe_free(node);
        return NULL;
    }

    return node;
}