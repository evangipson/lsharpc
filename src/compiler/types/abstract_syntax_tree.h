#ifndef ABSTRACT_SYNTAX_TREE
#define ABSTRACT_SYNTAX_TREE
#include "../extensions/collection_extensions.h"
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"

typedef enum abstract_syntax_node_type
{
    AST_NODE_PROGRAM,
    AST_NODE_STATEMENT,
    AST_NODE_DECLARATION,
    AST_NODE_ASSIGNMENT,
    AST_NODE_FUNCTION_CALL,
    AST_NODE_CONTRACT_DEFINITION,
    AST_NODE_RETURN_STATEMENT,
    AST_NODE_EXPRESSION,
    AST_NODE_TERM,
    AST_NODE_FACTOR,
    AST_NODE_LITERAL,
    AST_NODE_ARRAY_LITERAL,
    AST_NODE_TYPE,
    AST_NODE_NUMBER_LITERAL,
    AST_NODE_STRING_LITERAL,
    AST_NODE_BIT_LITERAL,
    AST_NODE_IDENTIFIER,
    AST_NODE_BINARY_OP,
    AST_NODE_TEMPLATE_STRING,
    AST_NODE_GRAB_STATEMENT,
    AST_NODE_ERROR
} abstract_syntax_node_type;

typedef struct abstract_syntax_node abstract_syntax_node;

struct abstract_syntax_node
{
    abstract_syntax_node_type type;
    struct abstract_syntax_node* left;
    struct abstract_syntax_node* right;
    struct abstract_syntax_node** children;
    int children_count;
    int children_capacity;

    union
    {
        struct
        {
            struct abstract_syntax_node* left;
            struct abstract_syntax_node* right;
            char* op_symbol;
        } binary_operation;

        struct
        {
            char* function_name;
            struct abstract_syntax_node** arguments;
            int argument_count;
            int argument_capacity;
        } function_call;

        struct
        {
            char* variable_name;
            struct abstract_syntax_node* expression;
        } assignment;

        struct
        {
            double value;
        } number_literal;

        struct
        {
            char* value;
        } string_literal;

        struct
        {
            bool value;
        } bit_literal;

        struct
        {
            struct abstract_syntax_node** elements;
            int element_count;
            int element_capacity;
        } array_literal;

        struct
        {
            char* type_name;
        } type_node;

        struct
        {
            char* identifier_name;
        } identifier_node;

        struct
        {
            struct abstract_syntax_node** statements;
            int statement_count;
            int statement_capacity;
        } program_node;

        struct
        {
            struct abstract_syntax_node* statement;
        } statement_node;

        struct
        {
            char* type_name;
            char* variable_name;
            struct abstract_syntax_node* expression;
        } declaration_node;

        struct
        {
            char* contract_name;
            struct abstract_syntax_node** declarations;
            int declaration_count;
            int declaration_capacity;
        } contract_definition_node;

        struct
        {
            struct abstract_syntax_node* expression;
        } return_statement_node;

        struct
        {
            struct abstract_syntax_node** parts;
            int part_count;
            int part_capacity;
        } template_string_node;

        struct
        {
            struct abstract_syntax_node* identifier;
        } grab_statement_node;

        struct
        {
            char* message;
            int line;
            int column;
            int code;
        } error_node;
    } data;
};

abstract_syntax_node* create_ast_node_binary_operation(abstract_syntax_node* left, abstract_syntax_node* right, char* op_symbol);

abstract_syntax_node* create_ast_node_function_call(char* function_name, abstract_syntax_node** arguments, int argument_count, int argument_capacity);

abstract_syntax_node* create_ast_node_assignment(char* variable_name, abstract_syntax_node* expression);

abstract_syntax_node* create_ast_node_number_literal(double value);

abstract_syntax_node* create_ast_node_string_literal(char* value);

abstract_syntax_node* create_ast_node_bit_literal(bool value);

abstract_syntax_node* create_ast_node_array_literal(abstract_syntax_node** elements, int element_count, int element_capacity);

abstract_syntax_node* create_ast_node_type(char* type_name);

abstract_syntax_node* create_ast_node_identifier(char* identifier_name);

abstract_syntax_node* create_ast_node_program(abstract_syntax_node** statements, int statement_count, int statement_capacity);

abstract_syntax_node* create_ast_node_statement(abstract_syntax_node* statement);

abstract_syntax_node* create_ast_node_declaration(char* type_name, char* variable_name, struct abstract_syntax_node* expression);

abstract_syntax_node* create_ast_node_contract_definition(char* contract_name, abstract_syntax_node** declarations, int declaration_count, int declaration_capacity);

abstract_syntax_node* create_ast_node_return_statement(abstract_syntax_node* expression);

abstract_syntax_node* create_ast_node_template_string(abstract_syntax_node** parts, int part_count, int part_capacity);

abstract_syntax_node* create_ast_node_grab_statement(abstract_syntax_node* identifier);

abstract_syntax_node* create_ast_node_error(const char* message, int line, int column, int code);

#endif