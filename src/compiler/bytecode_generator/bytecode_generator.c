#include "bytecode_generator.h"

bytecode_generator* create_bytecode_generator()
{
    bytecode_generator* generator = (bytecode_generator*)safe_malloc(sizeof(bytecode_generator));
    generator->instruction_capacity = 16;
    generator->instructions = (instruction*)safe_malloc(generator->instruction_capacity * sizeof(instruction));
    generator->instruction_count = 0;
    generator->symbols = create_symbol_table(100);
    generator->object_capacity = 100;
    generator->object_count = 0;
    generator->objects = (char**)safe_malloc(generator->object_capacity * sizeof(char*));
    return generator;
}

void emit_instruction(bytecode_generator* generator, instruction new_instruction)
{
    if (generator->instruction_count >= generator->instruction_capacity)
    {
        generator->instruction_capacity *= 2;
        generator->instructions = (instruction*)realloc(generator->instructions, generator->instruction_capacity * sizeof(instruction));
    }

    generator->instructions[generator->instruction_count++] = new_instruction;
}

void generate_bytecode(bytecode_generator* generator, abstract_syntax_node* node)
{
    /* guard against NULL abstract syntax nodes */
    if (node == NULL)
    {
        return;
    }

    /* generate bytecode for each type of abstract syntax node */
    switch (node->type)
    {
        case AST_NODE_PROGRAM:
        {
            for (int i = 0; i < node->data.program_node.statement_count; i++)
            {
                generate_bytecode(generator, node->data.program_node.statements[i]);
            }
            /* add HALT instruction at the end */
            emit_instruction(generator, (instruction){OP_HALT, {0}});
            break;
        }
        case AST_NODE_STATEMENT:
        {
            generate_bytecode(generator, node->data.statement_node.statement);
            break;
        }
        case AST_NODE_ASSIGNMENT:
        {
            generate_bytecode(generator, node->data.assignment.expression);
            int variable_index = generator->variable_count++;
            insert_symbol(generator->symbols, node->data.assignment.variable_name, SYMBOL_VARIABLE, variable_index);
            emit_instruction(generator, (instruction){OP_STORE_VAR, {.variable = {variable_index}}});
            break;
        }
        case AST_NODE_CONTRACT_DEFINITION:
        {
            /**
             * TODO: Implement contract handling here
             */
            break;
        }
        case AST_NODE_RETURN_STATEMENT:
        {
            if (node->data.return_statement_node.expression != NULL)
            {
                generate_bytecode(generator, node->data.return_statement_node.expression);
            }
            emit_instruction(generator, (instruction){OP_RETURN, {0}});
            break;
        }
        case AST_NODE_NUMBER_LITERAL:
        {
            log_debug("[generate_bytecode]: found number literal, adding it to bytecode objects");
            emit_instruction(generator, (instruction){OP_LOAD_CONST, {.d = node->data.number_literal.value}, OP_TYPE_NUMBER});
            break;
        }
        case AST_NODE_STRING_LITERAL:
        {
            log_debug("[generate_bytecode]: found string literal, adding it to bytecode objects");
            generator->objects = (char**)realloc(generator->objects, generator->object_count++ * sizeof(char*));
            generator->objects[generator->object_count] = duplicate_string(node->data.string_literal.value);
            log_debug("[generate_bytecode]: generator->object_count is %d", generator->object_count);
            log_debug("[generate_bytecode]: generator->objects[generator->object_count] is %s", (char*)generator->objects[generator->object_count]);

            /* store the index to the string object, not the string itself */
            log_debug("[generate_bytecode]: emitting OP_LOAD_CONST string index %d", generator->object_count);
            instruction* inst = (instruction*)safe_malloc(sizeof(instruction));
            if(inst == NULL)
            {
                log_error("[generate_bytecode]: unable to allocate memory for string literal instruction");
                safe_free(inst);
                break;
            }
            inst->op_code = OP_LOAD_CONST;
            inst->op_type = OP_TYPE_TEXT;
            inst->operand.i = generator->object_count;
            emit_instruction(generator, *inst);
            break;
        }
        case AST_NODE_TEMPLATE_STRING:
        {
            /**
             * TODO: Implement template string handling here
             */
            break;
        }
        case AST_NODE_BIT_LITERAL:
        {
            emit_instruction(generator, (instruction){OP_LOAD_CONST, {.b = node->data.bit_literal.value}, OP_TYPE_BIT});
            break;
        }
        case AST_NODE_ARRAY_LITERAL:
        {
            /**
             * TODO: Implement array literal handling here
             */
            break;
        }
        case AST_NODE_BINARY_OP:
        {
            generate_bytecode(generator, node->data.binary_operation.left);
            generate_bytecode(generator, node->data.binary_operation.right);
            op_code opcode;
            switch (*node->data.binary_operation.op_symbol)
            {
                case '+': opcode = OP_ADD; break;
                case '-': opcode = OP_SUB; break;
                case '*': opcode = OP_MUL; break;
                case '/': opcode = OP_DIV; break;
                default:
                    log_error("[generate_bytecode]: unknown binary operator.");
                    exit(1);
            }
            emit_instruction(generator, (instruction){opcode, {0}});
            break;
        }
        case AST_NODE_DECLARATION:
        {
            /* insert the symbol into the symbol table first */
            int variable_index = generator->variable_count++;

            symbol_type variable_type = SYMBOL_UNKNOWN;
            if (strcmp(node->data.declaration_node.type_name, "number") == 0)
            {
                variable_type = SYMBOL_NUMBER;
            }
            else if (strcmp(node->data.declaration_node.type_name, "text") == 0)
            {
                variable_type = SYMBOL_TEXT;
            }
            else if (strcmp(node->data.declaration_node.type_name, "bit") == 0)
            {
                variable_type = SYMBOL_BIT;
            }

            insert_symbol(generator->symbols, node->data.declaration_node.variable_name, SYMBOL_VARIABLE, variable_index);

            /* generate bytecode for the initializer expression */
            generate_bytecode(generator, node->data.declaration_node.expression);

            /* emit OP_STORE_VAR with the variable's index and type. */
            instruction store_var_instr = {OP_STORE_VAR, {.variable = {variable_index, variable_type}}, OP_TYPE_VARIABLE};
            emit_instruction(generator, store_var_instr);

            break;
        }
        case AST_NODE_FUNCTION_CALL:
        {
            /* push arguments onto the stack */
            for (int i = 0; i < node->data.function_call.argument_count; i++)
            {
                generate_bytecode(generator, node->data.function_call.arguments[i]);
            }
        
            /* handle built-in functions */
            int function_index = generator->function_count++;
            insert_symbol(generator->symbols, node->data.function_call.function_name, SYMBOL_FUNCTION, function_index);
            emit_instruction(generator, (instruction){OP_CALL, {.call = {function_index}}});
            break;
        }
        case AST_NODE_GRAB_STATEMENT:
        {
            /**
             * TODO: implement grab statement
             */
            break;
        }
        case AST_NODE_ERROR:
        {
            log_error("[generate_bytecode]: compilation error %d: %s", node->data.error_node.code, node->data.error_node.message);
            break;
        }
        default:
            log_error("[generate_bytecode]: unknown abstract syntax tree node type.");
            exit(1);
    }
}

instruction* compile_ast_to_bytecode(abstract_syntax_node* ast, int* instruction_count, const char* output_filename)
{
    bytecode_generator* generator = create_bytecode_generator();

    generate_bytecode(generator, ast);
    *instruction_count = generator->instruction_count;
    instruction* instructions = generator->instructions;

    write_bytecode_to_file(instructions, *instruction_count, generator->objects, generator->object_count, output_filename);
    safe_free(generator);

    return instructions;
}

void write_bytecode_to_file(instruction* instructions, int instruction_count, char** objects, int object_count, const char* filename)
{
    /* open the file in binary write mode */
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        log_error("[write_bytecode_to_file]: error opening file for writing");
        return;
    }

    // Write object_count
    fwrite(&object_count, sizeof(int), 1, file);

    // Write objects
    for (int i = 0; i < object_count; i++)
    {
        int string_length = strlen(objects[i]);
        fwrite(&string_length, sizeof(int), 1, file);
        fwrite(objects[i], sizeof(char), string_length, file);
    }

    /* write instruction count */
    fwrite(&instruction_count, sizeof(int), 1, file);

    /* write instructions */
    for (int i = 0; i < instruction_count; i++)
    {
        fwrite(&instructions[i].op_code, sizeof(op_code), 1, file);
        
        /* write out size of operand */
        size_t size_of_operand = sizeof(size_t);
        fwrite(&size_of_operand, sizeof(size_t), 1, file);
        /* write out operand itself */
        fwrite(&instructions[i].operand, sizeof(instructions[i].operand), 1, file);
        
        fwrite(&instructions[i].op_type, sizeof(op_type), 1, file);
    }

    fclose(file);
}

void free_bytecode_generator(bytecode_generator* generator)
{
    safe_free_collection((void**)generator->instructions);
    free_symbol_table(generator->symbols);
    safe_free(generator);
}