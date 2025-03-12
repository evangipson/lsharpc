#include "get_abstract_syntax_tree.h"

static token** token_stream;
static size_t token_count;
static int current_token_index;

/* function prototypes (forward declarations) */
static token* peek_token();
static token* peek_token_ahead(int lookahead);
static bool peek_token_is_type();
static token* consume_token();
static bool match_token(token_type expected_type);

static abstract_syntax_node* parse_factor_expression();
static abstract_syntax_node* parse_function_expression();
static abstract_syntax_node* parse_collecton_expression();
static abstract_syntax_node* parse_multiplicative_expression();
static abstract_syntax_node* parse_additive_expression();
static abstract_syntax_node* parse_expression();
static abstract_syntax_node* parse_contract_definition();
static abstract_syntax_node* parse_assignment();
static abstract_syntax_node* parse_declaration();
static abstract_syntax_node* parse_return_statement();
static abstract_syntax_node* parse_grab_statement();
static abstract_syntax_node* parse_statement();
static abstract_syntax_node* parse_program();

static token* peek_token()
{
    if (current_token_index < token_count)
    {
        return token_stream[current_token_index];
    }
    return NULL;
}

static token* peek_token_ahead(int lookahead)
{
    if (current_token_index + lookahead < token_count)
    {
        return token_stream[current_token_index + lookahead];
    }
    /* guard if lookahead goes beyond the token stream */
    return NULL;
}

static bool peek_token_is_type()
{
    /* TODO: add user types */
    token_type token_type = peek_token()->type;
    return token_type == TOKEN_TYPE_NUMBER
        || token_type == TOKEN_TYPE_TEXT
        || token_type == TOKEN_TYPE_BIT
        || token_type == TOKEN_TYPE_NUMBERS
        || token_type == TOKEN_TYPE_TEXTS
        || token_type == TOKEN_TYPE_BITS;
}

static token* consume_token()
{
    if (current_token_index < token_count)
    {
        return token_stream[current_token_index++];
    }
    return NULL;
}

static bool match_token(token_type expected_type)
{
    if (peek_token() != NULL && peek_token()->type == expected_type)
    {
        consume_token();
        return true;
    }
    return false;
}

static abstract_syntax_node* parse_factor_expression()
{
    /* guard against empty factor expressions */
    if (peek_token() == NULL)
    {
        return create_ast_node_error("[parser error]: unable to parse expression, empty factor", peek_token()->line, peek_token()->column, 4);
    }

    /* look for function calls */
    abstract_syntax_node* function_expression = parse_function_expression();
    if(function_expression != NULL)
    {
        return function_expression;
    }

    if (peek_token()->type == TOKEN_NUMBER_LITERAL)
    {
        double value = atof(consume_token()->value);
        return create_ast_node_number_literal(value);
    }
    else if (peek_token()->type == TOKEN_IDENTIFIER)
    {
        char* token_identifier_value = duplicate_string(consume_token()->value);
        if(token_identifier_value != NULL)
        {
            return create_ast_node_identifier(token_identifier_value);
        }

        /* handle error: unable to allocate memory identifier */
        return create_ast_node_error("[parser error]: unable to parse expression, error allocating memory for identifier", peek_token()->line, peek_token()->column, 22);
    }
    else if (peek_token()->type == TOKEN_PUNCTUATION_OPEN_PAREN)
    {
        /* consume "(" */
        consume_token();
        abstract_syntax_node* expression = parse_expression();
        if (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_CLOSE_PAREN)
        {
            /* consume ")" */
            consume_token();
            return expression;
        }

        /* handle error: expected ")" */
        return create_ast_node_error("[parser error]: unable to parse expression, expected closing parenthesis", peek_token()->line, peek_token()->column, 5);
    }
    else if (peek_token()->type == TOKEN_STRING_LITERAL)
    {
        char* string_literal_value = duplicate_string(consume_token()->value);
        if(string_literal_value != NULL)
        {
            return create_ast_node_string_literal(string_literal_value);
        }

        /* handle error: unable to allocate string literal value */
        return create_ast_node_error("[parser error]: unable to parse expression, error allocating memory for string literal value", peek_token()->line, peek_token()->column, 21);
    }
    else if (peek_token()->type == TOKEN_BIT_LITERAL_ON)
    {
        return create_ast_node_bit_literal(true);
    }
    else if (peek_token()->type == TOKEN_BIT_LITERAL_OFF)
    {
        return create_ast_node_bit_literal(false);
    }

    abstract_syntax_node* collection_expression = parse_collecton_expression();
    if (collection_expression != NULL)
    {
        return collection_expression;
    }

    return NULL;
}

static abstract_syntax_node* parse_function_expression()
{
    /* look for fluent (i.e. foo.bar()) function calls */
    if (peek_token()->type == TOKEN_IDENTIFIER && peek_token_ahead(1) != NULL && peek_token_ahead(1)->type == TOKEN_PUNCTUATION_PERIOD)
    {
        /* build the full function name (e.g., "io.log") */
        char* full_function_name = duplicate_string(peek_token()->value);
        /* consume the first identifier */
        consume_token();

        while (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_PERIOD) {
            /* consume the "." */
            consume_token();
            if (peek_token() != NULL && peek_token()->type == TOKEN_IDENTIFIER)
            {
                /* +1 for '.', +1 for null-terminator */
                char* temp = (char*)safe_malloc(strlen(full_function_name) + strlen(peek_token()->value) + 2);
                strcpy(temp, full_function_name);
                strcat(temp, ".");
                strcat(temp, peek_token()->value);
                safe_free(full_function_name);
                full_function_name = temp;
                /* consume the identifier */
                consume_token();
            }
            else
            {
                /* handle error: expected identifier after period */
                safe_free(full_function_name);
                return create_ast_node_error("[parser error]: expected identifier after period", peek_token()->line, peek_token()->column, 19);
            }
        }

        /* check for the opening parenthesis */
        if (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_OPEN_PAREN) {
            /* consume the opening parenthesis */
            consume_token();

            abstract_syntax_node** arguments = NULL;
            int argument_count = 0;
            int argument_capacity = 8;
            arguments = (abstract_syntax_node**)safe_malloc(argument_capacity * sizeof(abstract_syntax_node*));
            if (peek_token() != NULL && peek_token()->type != TOKEN_PUNCTUATION_CLOSE_PAREN)
            {
                arguments[argument_count++] = parse_expression();
                while (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_COMMA)
                {
                    consume_token();
                    arguments[argument_count++] = parse_expression();
                    if (argument_count >= argument_capacity) {
                        argument_capacity *= 2;
                        arguments = (abstract_syntax_node**)realloc(arguments, argument_capacity * sizeof(abstract_syntax_node*));
                    }
                }
            }

            if (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_CLOSE_PAREN)
            {
                consume_token();
                abstract_syntax_node* function_call_node = create_ast_node_function_call(full_function_name, arguments, argument_count, argument_capacity);
                safe_free(full_function_name);
                return function_call_node;
            }
            else
            {
                safe_free(full_function_name);
                return create_ast_node_error("[parser error]: expected closing parenthesis", peek_token()->line, peek_token()->column, 20);
            }
        }
        else
        {
            safe_free(full_function_name);
            return create_ast_node_error("[parser error]: expected opening parenthesis after fluent call", peek_token()->line, peek_token()->column, 21);
        }
    }
    /* look for normal (period-less) function calls */
    if (peek_token()->type == TOKEN_IDENTIFIER && peek_token_ahead(1) != NULL && peek_token_ahead(1)->type == TOKEN_PUNCTUATION_OPEN_PAREN)
    {
        /* consume identifier */
        char* function_name = duplicate_string(consume_token()->value);
        /* Consume "(" */
        consume_token();

        abstract_syntax_node** arguments = NULL;
        int argument_count = 0;
        /* initial capacity, can't have more the 8 arguments */
        int argument_capacity = 8;
        arguments = (abstract_syntax_node**)safe_malloc(argument_capacity * sizeof(abstract_syntax_node*));

        if (peek_token() != NULL && peek_token()->type != TOKEN_PUNCTUATION_CLOSE_PAREN)
        {
            /* parse first argument */
            arguments[argument_count++] = parse_expression();

            /* parse subsequent arguments */
            while (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_COMMA)
            {
                /* consume "," */
                consume_token();
                arguments[argument_count++] = parse_expression();
                if (argument_count >= argument_capacity)
                {
                    argument_capacity *= 2;
                    arguments = (abstract_syntax_node**)realloc(arguments, argument_capacity * sizeof(abstract_syntax_node*));
                }
            }
        }

        if (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_CLOSE_PAREN)
        {
            /* Consume "(" */
            consume_token();
            abstract_syntax_node* function_call_node = create_ast_node_function_call(function_name, arguments, argument_count, argument_capacity);
            safe_free(function_name);
            return function_call_node;
        }
        else
        {
            /* handle error: expected ")" */
            return create_ast_node_error("[parser error]: unable to parse function, expected closing parenthesis", peek_token()->line, peek_token()->column, 6);
            safe_free(function_name);
        }
    }
    
    return NULL;
}

static abstract_syntax_node* parse_collecton_expression()
{
    if (peek_token()->type == TOKEN_PUNCTUATION_OPEN_BRACKET)
    {
        /* consume "[" */
        consume_token();

        abstract_syntax_node** elements = NULL;
        int element_count = 0;
        /* initial capacity */
        int element_capacity = 4;
        elements = (abstract_syntax_node**)safe_malloc(element_capacity * sizeof(abstract_syntax_node*));

        if (peek_token() != NULL && peek_token()->type != TOKEN_PUNCTUATION_CLOSE_BRACKET)
        {
            /* parse first element */
            elements[element_count++] = parse_expression();

            /* parse subsequent elements */
            while (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_COMMA)
            {
                /* consume "," */
                consume_token();
                elements[element_count++] = parse_expression();
                if (element_count >= element_capacity) {
                    element_capacity *= 2;
                    elements = (abstract_syntax_node**)realloc(elements, element_capacity * sizeof(abstract_syntax_node*));
                }
            }
        }

        if (peek_token() != NULL && peek_token()->type == TOKEN_PUNCTUATION_CLOSE_BRACKET)
        {
            /* consume "]" */
            consume_token();
            abstract_syntax_node* array_literal_node = create_ast_node_array_literal(elements, element_count, element_capacity);
            return array_literal_node;
        }

        /* handle error: Expected "]" */
        return create_ast_node_error("[parser error]: unable to parse array literal, expected \"]\"", peek_token()->line, peek_token()->column, 7);
    }

    return NULL;
}

static abstract_syntax_node* parse_multiplicative_expression()
{
    /* is there a higher precedence? */
    abstract_syntax_node* left = parse_factor_expression();

    while (peek_token() != NULL && (peek_token()->type == TOKEN_OPERATOR_MULTIPLY || peek_token()->type == TOKEN_OPERATOR_DIVIDE))
    {
        char* op_symbol = strdup(consume_token()->value);
        abstract_syntax_node* right = parse_factor_expression();
        left = create_ast_node_binary_operation(left, right, op_symbol);
        safe_free(op_symbol);
    }
    return left;
}

static abstract_syntax_node* parse_additive_expression()
{
    /* is there a higher precedence? */
    abstract_syntax_node* left = parse_multiplicative_expression();

    while (peek_token() != NULL && (peek_token()->type == TOKEN_OPERATOR_PLUS || peek_token()->type == TOKEN_OPERATOR_MINUS))
    {
        char* op_symbol = strdup(consume_token()->value);
        abstract_syntax_node* right = parse_multiplicative_expression();
        left = create_ast_node_binary_operation(left, right, op_symbol);
        safe_free(op_symbol);
    }
    return left;
}

static abstract_syntax_node* parse_expression()
{
    /* start with the lowest precedence */
    return parse_additive_expression();
}

static abstract_syntax_node* parse_contract_definition()
{
    /* check for "contract" keyword */
    if (peek_token() == NULL || peek_token()->type != TOKEN_KEYWORD_CONTRACT)
    {
        /* handle error: expected "contract" keyword */
        return create_ast_node_error("[parser error]: expected 'contract' keyword", peek_token()->line, peek_token()->column, 13);
    }
    /* consume "contract" */
    consume_token();

    /* check for identifier (contract name) */
    if (peek_token() == NULL || peek_token()->type != TOKEN_IDENTIFIER)
    {
        /* handle error: expected contract name */
        return create_ast_node_error("[parser error]: expected contract name", peek_token()->line, peek_token()->column, 14);
    }
    char* contract_name = duplicate_string(consume_token()->value);
    /* consume contract name */
    consume_token();

    /* check for opening curly brace "{" */
    if (peek_token() == NULL || peek_token()->type != TOKEN_PUNCTUATION_OPEN_BRACKET) {
        /* handle error: Expected "{" */
        safe_free(contract_name);
        return create_ast_node_error("[parser error]: expected '{'", peek_token()->line, peek_token()->column, 15);
    }
    /* consume "{" */
    consume_token();

    /* parse declarations within the contract */
    abstract_syntax_node** declarations = NULL;
    int declaration_count = 0;
    /* initial capacity */
    int declaration_capacity = 4;
    declarations = (abstract_syntax_node**)safe_malloc(declaration_capacity * sizeof(abstract_syntax_node*));

    while (peek_token() != NULL && peek_token()->type != TOKEN_PUNCTUATION_CLOSE_BRACKET)
    {
        abstract_syntax_node* declaration = parse_declaration();
        if (declaration == NULL)
        {
            /* handle error: failed to parse declaration */
            return create_ast_node_error("[parser error]: failed to parse declaration", peek_token()->line, peek_token()->column, 16);
            /* skip the current token (error recovery) */
            consume_token();
        }

        declarations[declaration_count++] = declaration;
        if (declaration_count >= declaration_capacity)
        {
            declaration_capacity *= 2;
            declarations = (abstract_syntax_node**)realloc(declarations, declaration_capacity * sizeof(abstract_syntax_node*));
        }
    }

    /* check for closing curly brace "}" */
    if (peek_token() == NULL || peek_token()->type != TOKEN_PUNCTUATION_CLOSE_BRACKET)
    {
        /* handle error: Expected "}" */
        safe_free(contract_name);
        safe_free(declarations);
        return create_ast_node_error("[parser error]: expected '}'", peek_token()->line, peek_token()->column, 17);
    }
    /* consume "}" */
    consume_token();

    /* create and return the contract definition node */
    abstract_syntax_node* contract_node = create_ast_node_contract_definition(contract_name, declarations, declaration_count, declaration_capacity);
    safe_free(contract_name);
    return contract_node;
}

static abstract_syntax_node* parse_assignment()
{
    /* check for identifier */
    if (peek_token() == NULL || peek_token()->type != TOKEN_IDENTIFIER)
    {
        /* handle error: expected identifier */
        return create_ast_node_error("[parser error]: unable to parse assignment, expected identifier", peek_token()->line, peek_token()->column, 1);
    }
    char* variable_name = duplicate_string(consume_token()->value);

    /* check for assignment operator "=" */
    if (peek_token() == NULL || peek_token()->type != TOKEN_OPERATOR_ASSIGN)
    {
        /* handle error: Expected assignment operator */
        safe_free(variable_name);
        return create_ast_node_error("[parser error]: unable to parse assignment, expected assignment operator", peek_token()->line, peek_token()->column, 2);
    }

    /* consume "=" */
    consume_token();

    /* parse the expression */
    abstract_syntax_node* expression = parse_expression();

    /* create and return the assignment node */
    if (expression == NULL)
    {
        /* handle error: Failed to parse expression */
        safe_free(variable_name);
        return create_ast_node_error("[parser error]: unable to parse assignment, failed to parse expression", peek_token()->line, peek_token()->column, 3);
    }

    abstract_syntax_node* assignment_node = create_ast_node_assignment(variable_name, expression);

    /* free the duplicate as it is now inside of the AST. */
    safe_free(variable_name);
    
    return assignment_node;
}

static abstract_syntax_node* parse_declaration()
{
    /* check for type */
    if (peek_token() == NULL || peek_token_is_type() == false)
    {
        /* handle error: expected type */
        return create_ast_node_error("[parser error]: expected type in declaration", peek_token()->line, peek_token()->column, 7);
    }
    char* type_name = duplicate_string(consume_token()->value);

    /* check for identifier */
    if (peek_token() == NULL || peek_token()->type != TOKEN_IDENTIFIER)
    {
        /* handle error: Expected identifier */
        return create_ast_node_error("[parser error]: expected identifier in declaration", peek_token()->line, peek_token()->column, 8);
        safe_free(type_name);
    }
    char* variable_name = duplicate_string(consume_token()->value);

    /* check for optional assignment */
    abstract_syntax_node* expression = NULL;
    if (peek_token() != NULL && peek_token()->type == TOKEN_OPERATOR_ASSIGN)
    {
        /* consume "=" */
        consume_token();
        /* parse the expression */
        expression = parse_expression();
        if (expression == NULL)
        {
            /* handle error: expected expression after assignment */
            safe_free(type_name);
            safe_free(variable_name);
            return create_ast_node_error("[parser error]: expected expression after assignment in declaration", peek_token()->line, peek_token()->column, 9);
        }
    }

    /* create and return the declaration node */
    abstract_syntax_node* declaration_node = create_ast_node_declaration(type_name, variable_name, expression);

    safe_free(type_name);
    safe_free(variable_name);

    return declaration_node;
}

static abstract_syntax_node* parse_return_statement()
{
    /* check for "return" keyword */
    if (peek_token() == NULL || peek_token()->type != TOKEN_KEYWORD_RETURN)
    {
        /* handle error: expected "return" keyword */
        return create_ast_node_error("[parser error]: expected 'return' keyword", peek_token()->line, peek_token()->column, 11);
    }
    /* consume "return" */
    consume_token();

    /* parse the expression */
    abstract_syntax_node* expression = parse_expression();
    if (expression == NULL)
    {
        /* handle error: failed to parse expression */
        return create_ast_node_error("[parser error]: failed to parse expression after 'return'", peek_token()->line, peek_token()->column, 12);
    }

    /* create and return the return statement node */
    return create_ast_node_return_statement(expression);
}

static abstract_syntax_node* parse_grab_statement()
{
    /* consume the "grab" token */
    consume_token();
    if (peek_token()->type == TOKEN_IDENTIFIER)
    {
        /* create the identifier node for the grab statement */
        abstract_syntax_node* identifier = create_ast_node_identifier(peek_token()->value);
        /* consume the identifier token */
        consume_token();

        return create_ast_node_grab_statement(identifier->data.identifier_node.identifier_name);
    }

    /* handle error: expected grab to have an identifier */
    return create_ast_node_error("[parser error]: expected identifier after grab", peek_token()->line, peek_token()->column, 19);
}

static abstract_syntax_node* parse_statement()
{
    if (peek_token() == NULL)
    {
        return NULL;
    }

    if (peek_token()->type == TOKEN_KEYWORD_GRAB)
    {
        return parse_grab_statement();
    }

    if (peek_token_is_type() == true)
    {
        return parse_declaration();
    }
    
    if (peek_token()->type == TOKEN_IDENTIFIER && peek_token_ahead(1) != NULL && peek_token_ahead(1)->type == TOKEN_OPERATOR_ASSIGN)
    {
        return parse_assignment();
    }
    
    if (peek_token()->type == TOKEN_IDENTIFIER && peek_token_ahead(1) != NULL && peek_token_ahead(1)->type == TOKEN_PUNCTUATION_OPEN_PAREN)
    {
        return parse_function_expression();
    }
    
    if (peek_token()->type == TOKEN_KEYWORD_RETURN)
    {
        return parse_return_statement();
    }
    
    if (peek_token()->type == TOKEN_KEYWORD_CONTRACT)
    {
        return parse_contract_definition();
    }

    /* if we didn't match any token types, we have an expression */
    abstract_syntax_node* expression = parse_expression();
    if (expression != NULL)
    {
        return expression;
    }

    /* TODO: Add more statement parsing logic here (e.g., if statements, while loops) */
    
    return NULL;
}

static abstract_syntax_node* parse_program()
{
    /* initial capacity of 50 syntax nodes */
    abstract_syntax_node* program_node = create_ast_node_program(NULL, 0, 50);
    program_node->data.program_node.statements = (abstract_syntax_node**)safe_malloc(program_node->data.program_node.statement_capacity * sizeof(abstract_syntax_node*));

    log_debug("[parse_program]: starting the parser");

    while (peek_token() != NULL && peek_token()->type != TOKEN_END_OF_FILE)
    {
        abstract_syntax_node* statement_node = parse_statement();
        if (statement_node != NULL)
        {
            program_node->data.program_node.statements[program_node->data.program_node.statement_count] = statement_node;
            program_node->data.program_node.statement_count++;
            if (program_node->data.program_node.statement_count >= program_node->data.program_node.statement_capacity)
            {
                program_node->data.program_node.statement_capacity *= 2;
                program_node->data.program_node.statements = (abstract_syntax_node**)realloc(program_node->data.program_node.statements, program_node->data.program_node.statement_capacity * sizeof(abstract_syntax_node*));
            }
        }
        else
        {
            /* handle error: failed to parse statement */
            abstract_syntax_node* error_node =  create_ast_node_error("[parser error]: failed to parse statement", peek_token()->line, peek_token()->column, 10);
            program_node->data.program_node.statements[program_node->data.program_node.statement_count] = error_node;
            program_node->data.program_node.statement_count++;

            /* skip the current token (error recovery) */
            consume_token();
        }
    }

    log_debug("[parse_program]: program node created, statement_count = %d", program_node->data.program_node.statement_count);

    return program_node;
}

abstract_syntax_node* get_abstract_syntax_tree(token** tokens, size_t number_of_tokens)
{
    token_stream = tokens;
    current_token_index = 0;
    token_count = number_of_tokens;
    return parse_program();
}

void free_ast(abstract_syntax_node* node)
{
    /* guard against null nodes being freed */
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
        case AST_NODE_BINARY_OP:
            free_ast(node->data.binary_operation.left);
            free_ast(node->data.binary_operation.right);
            safe_free(node->data.binary_operation.op_symbol);
            break;
        case AST_NODE_FUNCTION_CALL:
            safe_free(node->data.function_call.function_name);
            for (int i = 0; i < node->data.function_call.argument_count; i++)
            {
                free_ast(node->data.function_call.arguments[i]);
            }
            safe_free(node->data.function_call.arguments);
            break;
        case AST_NODE_ASSIGNMENT:
            safe_free(node->data.assignment.variable_name);
            free_ast(node->data.assignment.expression);
            break;
        case AST_NODE_NUMBER_LITERAL:
            /* nothing to free for number literals */
            break;
        case AST_NODE_STRING_LITERAL:
            safe_free(node->data.string_literal.value);
            break;
        case AST_NODE_BIT_LITERAL:
            /* nothing to free for bit literals */
            break;
        case AST_NODE_ARRAY_LITERAL:
            for (int i = 0; i < node->data.array_literal.element_count; i++)
            {
                free_ast(node->data.array_literal.elements[i]);
            }
            safe_free(node->data.array_literal.elements);
            break;
        case AST_NODE_TYPE:
            safe_free(node->data.type_node.type_name);
            break;
        case AST_NODE_IDENTIFIER:
            safe_free(node->data.identifier_node.identifier_name);
            break;
        case AST_NODE_PROGRAM:
            for (int i = 0; i < node->data.program_node.statement_count; i++)
            {
                free_ast(node->data.program_node.statements[i]);
            }
            safe_free(node->data.program_node.statements);
            break;
        case AST_NODE_STATEMENT:
            free_ast(node->data.statement_node.statement);
            break;
        case AST_NODE_CONTRACT_DEFINITION:
            safe_free(node->data.contract_definition_node.contract_name);
            for (int i = 0; i < node->data.contract_definition_node.declaration_count; i++)
            {
                free_ast(node->data.contract_definition_node.declarations[i]);
            }
            safe_free(node->data.contract_definition_node.declarations);
            break;
        case AST_NODE_RETURN_STATEMENT:
            free_ast(node->data.return_statement_node.expression);
            break;
        case AST_NODE_TEMPLATE_STRING:
            for(int i = 0; i < node->data.template_string_node.part_count; i++)
            {
                free_ast(node->data.template_string_node.parts[i]);
            }
            safe_free(node->data.template_string_node.parts);
            break;
        case AST_NODE_ERROR:
            safe_free(node->data.error_node.message);
            break;
        case AST_NODE_DECLARATION:
            safe_free(node->data.assignment.variable_name);
            safe_free(node->data.type_node.type_name);
            free_ast(node->data.assignment.expression);
            break;
        default:
            /* TODO: Handle other node types or do nothing if there's no data to free */
            break;
    }

    /* free the node itself */
    safe_free(node);
}