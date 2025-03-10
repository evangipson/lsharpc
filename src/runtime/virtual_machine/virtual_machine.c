#include "virtual_machine.h"

/// @brief helper function to mark an object
static void mark_object(virtual_machine* vm, int index)
{
    if (vm->object_flags[index] == 0)
    {
        vm->object_flags[index] = 1;
        /**
         * TODO: If your objects contain pointers to other objects, recursively mark them here.
         * 
         * For example, if you have a linked list:
         * 
         * if (object_is_list(vm->objects[index]))
         * {
         *     mark_object(vm, get_list_head_index(vm->objects[index]));
         * }
         */
    }
}

bool create_virtual_machine(virtual_machine* vm)
{
    /* initialize VM components */
    vm->stack_size = 256;
    vm->stack = (value*)safe_malloc(vm->stack_size * sizeof(value));
    vm->stack_pointer = 0;

    /* will be set by load_bytecode_from_file */
    vm->instruction_capacity = 0;
    vm->instruction_count = 0;
    vm->instructions = NULL;

    vm->variable_count = 256;
    vm->variables = (value*)safe_malloc(vm->variable_count * sizeof(value));

    /* will be set by load_bytecode_from_file */
    vm->object_capacity = 0;
    vm->object_count = 0;
    vm->objects = NULL;

    vm->object_flags = (char*)calloc(vm->object_capacity, sizeof(char));

    vm->program_counter = 0;

    return true;
}

bool load_bytecode_from_file(virtual_machine* vm, const char* filename)
{
    /* open in binary read mode */
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        log_error("[read_bytecode_from_file]: error opening \"%s\" bytecode file for reading", filename);
        return false;
    }

    /* read object_count */
    int object_count;
    fread(&object_count, sizeof(int), 1, file);

    /* read objects */
    vm->object_count = object_count;
    vm->instruction_capacity = object_count;
    vm->objects = (char**)safe_malloc(object_count * sizeof(char*));
    if (vm->objects == NULL)
    {
        log_error("[read_bytecode_from_file]: error allocating memory for objects");
        fclose(file);
        safe_free(vm);
        return NULL;
    }

    for (int i = 0; i < vm->object_count; i++)
    {
        int string_length;
        fread(&string_length, sizeof(int), 1, file);
        /* +1 for null terminator */
        vm->objects[i] = (char*)malloc((string_length + 1) * sizeof(char));
        if (vm->objects[i] == NULL)
        {
            log_error("[read_bytecode_from_file]: error allocating memory for object");
            fclose(file);
            safe_free(vm);
            return NULL;
        }
        fread(vm->objects[i], sizeof(char), string_length, file);
        /* null-terminate the string */
        vm->objects[i][string_length] = '\0';
    }

    /* read instruction count */
    int instruction_count;
    fread(&instruction_count, sizeof(int), 1, file);

    /* read instructions */
    vm->instruction_count = instruction_count;
    vm->instruction_capacity = instruction_count;
    vm->instructions = (instruction*)safe_malloc(instruction_count * sizeof(instruction));
    if (vm->instructions == NULL)
    {
        log_error("[read_bytecode_from_file]: error allocating memory for instructions");
        fclose(file);
        safe_free(vm);
        return NULL;
    }

    for (int i = 0; i < instruction_count; i++)
    {
        fread(&vm->instructions[i].op_code, sizeof(op_code), 1, file);
        log_debug("[read_bytecode_from_file]: read an op code: %s", get_op_code_name(vm->instructions[i].op_code));

        size_t size_of_operand = 0;
        fread(&size_of_operand, sizeof(size_t), 1, file);
        log_debug("[read_bytecode_from_file]: read size of operand: %zu", size_of_operand);

        fread(&vm->instructions[i].operand, size_of_operand, 1, file);

        fread(&vm->instructions[i].op_type, sizeof(op_type), 1, file);
        log_debug("[read_bytecode_from_file]: read an op type: %s", get_op_type_name(vm->instructions[i].op_type));
    }

    fclose(file);
    return true;
}

bool run_vm(virtual_machine* vm)
{
    while (vm->program_counter < vm->instruction_count)
    {
        instruction instruction = vm->instructions[vm->program_counter];
        vm->program_counter++;

        switch (instruction.op_code)
        {
            case OP_LOAD_CONST:
            {
                log_debug("[run_vm]: vm->object_count is %d", vm->object_count);

                value new_value;
                /* determine the type of the constant and load it */
                switch (instruction.op_type)
                {
                    case OP_TYPE_INDEX:
                    case OP_TYPE_TEXT:
                    {
                        log_debug("[run_vm]: OP_LOAD_CONST operand.i is %d (string index)", instruction.operand.i);
                        new_value.type = VAL_STRING;
                        new_value.as.i = instruction.operand.i;
                        break;
                    }
                    case OP_TYPE_NUMBER:
                    {
                        log_debug("[run_vm]: OP_LOAD_CONST operand.d is %d", instruction.operand.d);
                        new_value.type = VAL_DOUBLE;
                        new_value.as.d = instruction.operand.d;
                        break;
                    }
                    case OP_TYPE_BIT:
                    {
                        log_debug("[run_vm]: OP_LOAD_CONST operand.b is \"%s\"", instruction.operand.b ? "true" : "false");
                        new_value.type = VAL_BOOL;
                        new_value.as.b = instruction.operand.b;
                    }
                    default:
                    {
                        log_warning("[run_vm]: OP_LOAD_CONST operand didn't have a value for d, i, b, or s");
                        new_value.type = VAL_NULL;
                    }
                }

                vm->stack[vm->stack_pointer++] = new_value;
                break;
            }
            case OP_LOAD_VAR:
            {
                vm->stack[vm->stack_pointer++] = vm->variables[instruction.operand.variable.variable_index];
                break;
            }
            case OP_STORE_VAR:
            {
                vm->variables[instruction.operand.variable.variable_index] = vm->stack[--vm->stack_pointer];
                break;
            }
            case OP_POP:
            {
                vm->stack_pointer--;
                break;
            }
            case OP_ADD:
            {
                value b = vm->stack[--vm->stack_pointer];
                value a = vm->stack[--vm->stack_pointer];
                if (a.type == VAL_DOUBLE && b.type == VAL_DOUBLE)
                {
                    vm->stack[vm->stack_pointer++] = (value){VAL_DOUBLE, {.d = a.as.d + b.as.d}};
                }
                else
                {
                    log_error("Error: Invalid operand types for ADD.");
                    return false;
                }
                break;
            }
            case OP_SUB:
            {
                value b = vm->stack[--vm->stack_pointer];
                value a = vm->stack[--vm->stack_pointer];
                if (a.type == VAL_DOUBLE && b.type == VAL_DOUBLE)
                {
                    vm->stack[vm->stack_pointer++] = (value){VAL_DOUBLE, {.d = a.as.d - b.as.d}};
                }
                else
                {
                    log_error("Error: Invalid operand types for SUB.");
                    return false;
                }
                break;
            }
            case OP_MUL:
            {
                value b = vm->stack[--vm->stack_pointer];
                value a = vm->stack[--vm->stack_pointer];
                if (a.type == VAL_DOUBLE && b.type == VAL_DOUBLE)
                {
                    vm->stack[vm->stack_pointer++] = (value){VAL_DOUBLE, {.d = a.as.d * b.as.d}};
                }
                else
                {
                    log_error("Error: Invalid operand types for MUL.\n");
                    return false;
                }
                break;
            }
            case OP_DIV:
            {
                value b = vm->stack[--vm->stack_pointer];
                value a = vm->stack[--vm->stack_pointer];
                if (a.type == VAL_DOUBLE && b.type == VAL_DOUBLE)
                {
                    if (b.as.d == 0.0)
                    {
                        log_error("Error: Division by zero.");
                        return false;
                    }
                    vm->stack[vm->stack_pointer++] = (value){VAL_DOUBLE, {.d = a.as.d / b.as.d}};
                }
                else
                {
                    log_error("Error: Invalid operand types for DIV.");
                    return false;
                }
                break;
            }
            case OP_EQ:
            {
                value b = vm->stack[--vm->stack_pointer];
                value a = vm->stack[--vm->stack_pointer];
                if (a.type == b.type)
                {
                    uint8_t result = 0;
                    if (a.type == VAL_DOUBLE)
                    {
                        result = (a.as.d == b.as.d);
                    }
                    else if (a.type == VAL_INT)
                    {
                        result = (a.as.i == b.as.i);
                    }
                    vm->stack[vm->stack_pointer++] = (value){VAL_BOOL, {.b = result}};
                }
                else
                {
                    log_error("Error: Invalid operand types for EQ.");
                    return false;
                }
                break;
            }
            case OP_NEQ:
            {
                // Implement NEQ
                break;
            }
            case OP_GT:
            {
                // Implement GT
                break;
            }
            case OP_LT:
            {
                // Implement LT
                break;
            }
            case OP_AND:
            {
                // Implement AND
                break;
            }
            case OP_OR:
            {
                // Implement OR
                break;
            }
            case OP_NOT:
            {
                // Implement NOT
                break;
            }
            case OP_JMP:
            {
                vm->program_counter += instruction.operand.jump.jump_offset;
                break;
            }
            case OP_JMP_IF_FALSE:
            {
                value condition = vm->stack[--vm->stack_pointer];
                if (condition.type == VAL_BOOL && condition.as.b == 0)
                {
                    vm->program_counter += instruction.operand.jump.jump_offset;
                }
                break;
            }
            case OP_CALL:
            {
                // Implement CALL
                break;
            }
            case OP_RETURN:
            {
                // Implement RETURN
                break;
            }
            case OP_HALT:
            {
                /* stop execution */
                return true;
            }
            case OP_PRINT:
            {
                value value = vm->stack[--vm->stack_pointer];
                if (value.type == VAL_DOUBLE)
                {
                    printf("%f\n", value.as.d);
                }
                else if (value.type == VAL_STRING)
                {
                    log_debug("[run_vm]: finding string at index %d from objects", value.as.i);
                    /* retrieve string from objects array using the index */
                    char* string_to_print = (char*)vm->objects[value.as.i];
                    log_debug("[run_vm]: printing string %s", string_to_print);
                    printf("%s\n", string_to_print);
                }
                else if (value.type == VAL_BOOL)
                {
                    printf("%s\n", value.as.b ? "true" : "false");
                }
                break;
            }
            default:
            {
                log_error("Error: Unknown opcode %d.", instruction.op_code);
                return false;
            }
        }
    }

    return false;
}

void garbage_collect(virtual_machine* virtual_machine)
{
    /* mark objects reachable from the stack */
    for (int i = 0; i < virtual_machine->stack_pointer; i++)
    {
        if (virtual_machine->stack[i].type == VAL_STRING)
        {
            /* find the index of the string in the objects array */
            for (int j = 0; j < virtual_machine->object_count; j++)
            {
                if (virtual_machine->objects[j] == virtual_machine->stack[i].as.s)
                {
                    mark_object(virtual_machine, j);
                    break;
                }
            }
        }
    }

    /* mark objects reachable from variables */
    for (int i = 0; i < virtual_machine->variable_count; i++)
    {
        if (virtual_machine->variables[i].type == VAL_STRING)
        {
            /* find the index of the string in the objects array */
            for (int j = 0; j < virtual_machine->object_count; j++)
            {
                if (virtual_machine->objects[j] == virtual_machine->variables[i].as.s)
                {
                    mark_object(virtual_machine, j);
                    break;
                }
            }
        }
    }

    /* sweep objects when they are marked */
    int new_object_count = 0;
    for (int i = 0; i < virtual_machine->object_count; i++)
    {
        if (virtual_machine->object_flags[i] == 1)
        {
            /* marked, keep it */
            virtual_machine->objects[new_object_count] = virtual_machine->objects[i];
            /* unmark it for the next GC */
            virtual_machine->object_flags[new_object_count] = 0;
            new_object_count++;
            continue;
        }

        /* not marked, free it */
        safe_free(virtual_machine->objects[i]);
    }

    /* update object count post-sweep */
    virtual_machine->object_count = new_object_count;
}