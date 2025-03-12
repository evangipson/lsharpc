#ifndef ABSTRACT_SYNTAX_TREE
#define ABSTRACT_SYNTAX_TREE
#include "../../core/extensions/collection_extensions.h"
#include "../../core/extensions/memory_extensions.h"
#include "../../core/extensions/string_extensions.h"

/// @enum abstract_syntax_node_type
/// @brief A collection of abstract syntax node types for L#.
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

/// @struct abstract_syntax_node
/// @brief A node that represents different L# statements.
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
            char* module_name;
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

/// @brief Prints an abstract syntax tree node.
/// @param node The node to print.
/// @param indent The level of indentation to print the node with.
void print_ast_node(abstract_syntax_node* node, int indent);

/// @brief Gets a human-readable name of an `abstract_syntax_node_type`.
/// @param type The type to get a human-readable name of.
/// @return The name of the `type`, or `UNKNOWN` by default.
const char* get_ast_node_type_name(abstract_syntax_node_type type);

/// @brief Creates an `AST_NODE_BINARY_OP` abstract syntax node.
/// @param left The left side of the binary operation.
/// @param right The right side of the binary operation.
/// @param op_symbol The symbol of the binary operation.
/// @return A binary operation abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_binary_operation(abstract_syntax_node* left, abstract_syntax_node* right, char* op_symbol);

/// @brief Creates an `AST_NODE_FUNCTION_CALL` abstract syntax node.
/// @param function_name The name of the function to be called.
/// @param arguments The arguments of the function.
/// @param argument_count The number of arguments for the function.
/// @param argument_capacity The total amount of possible arguments for the function.
/// @return A function call abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_function_call(char* function_name, abstract_syntax_node** arguments, int argument_count, int argument_capacity);

/// @brief Creates an `AST_NODE_ASSIGNMENT` abstract syntax node.
/// @param variable_name The name of the variable that is being assigned to.
/// @param expression The expression that is being assigned to the variable.
/// @return An assignment abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_assignment(char* variable_name, abstract_syntax_node* expression);

/// @brief Creates an `AST_NODE_NUMBER_LITERAL` abstract syntax node.
/// @param value The value of the number literal.
/// @return A number literal abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_number_literal(double value);

/// @brief Creates an `AST_NODE_STRING_LITERAL` abstract syntax node.
/// @param value The value of the string literal.
/// @return A string literal abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_string_literal(char* value);

/// @brief Creates an `AST_NODE_BIT_LITERAL` abstract syntax node.
/// @param value The value of the bit literal.
/// @return A bit literal abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_bit_literal(bool value);

/// @brief Creates an `AST_NODE_ARRAY_LITERAL` abstract syntax node.
/// @param elements The elements of the array literal.
/// @param element_count The number of elements in the array literal.
/// @param element_capacity The total amount of possible elements of the array literal.
/// @return An array literal abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_array_literal(abstract_syntax_node** elements, int element_count, int element_capacity);

/// @brief Creates an `AST_NODE_TYPE` abstract syntax node.
/// @param type_name The name of the type.
/// @return A type abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_type(char* type_name);

/// @brief Creates an `AST_NODE_IDENTIFIER` abstract syntax node.
/// @param identifier_name The name of the identifier.
/// @return An identifier abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_identifier(char* identifier_name);

/// @brief Creates an `AST_NODE_PROGRAM` abstract syntax node.
/// @param statements The statements of the program.
/// @param statement_count The number of statements in the program.
/// @param statement_capacity The total amount of possible statements of the program.
/// @return A program abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_program(abstract_syntax_node** statements, int statement_count, int statement_capacity);

/// @brief Creates an `AST_NODE_STATEMENT` abstract syntax node.
/// @param statement A statement that represents this node.
/// @return A statement abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_statement(abstract_syntax_node* statement);

/// @brief Creates an `AST_NODE_DECLARATION` abstract syntax node.
/// @param type_name The name of the type for the declaration.
/// @param variable_name The name of the variable for the declaration.
/// @param expression The expression being assigned for the declaration.
/// @return A declaration abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_declaration(char* type_name, char* variable_name, struct abstract_syntax_node* expression);

/// @brief Creates an `AST_NODE_CONTRACT_DEFINITION` abstract syntax node.
/// @param contract_name The name of the contract.
/// @param declarations The declarations of the contract.
/// @param declaration_count The number of declarations of the contract.
/// @param declaration_capacity The total amount of possible declarations of the contract.
/// @return A contract definition abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_contract_definition(char* contract_name, abstract_syntax_node** declarations, int declaration_count, int declaration_capacity);

/// @brief Creates an `AST_NODE_RETURN_STATEMENT` abstract syntax node.
/// @param expression The expression that represents this statement.
/// @return A statement abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_return_statement(abstract_syntax_node* expression);

/// @brief Creates an `AST_NODE_TEMPLATE_STRING` abstract syntax node.
/// @param parts The parts of the template string.
/// @param part_count The number of parts in the template string.
/// @param part_capacity The total amount of possible parts of the template string.
/// @return A string template abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_template_string(abstract_syntax_node** parts, int part_count, int part_capacity);

/// @brief Creates an `AST_NODE_GRAB_STATEMENT` abstract syntax node.
/// @param module_name The name of the module being grabbed.
/// @return A grab statement abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_grab_statement(char* module_name);

/// @brief Creates an `AST_NODE_ERROR` abstract syntax node.
/// @param message The error message.
/// @param line The line number the error originated from.
/// @param column The column number the error originated from.
/// @param code The error code.
/// @return An error abstract syntax node, or `NULL` if creation failed.
abstract_syntax_node* create_ast_node_error(const char* message, int line, int column, int code);

#endif