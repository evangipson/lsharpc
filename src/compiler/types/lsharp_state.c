#include "lsharp_state.h"

static void free_compiler_error(compiler_error* compiler_error)
{
    if(compiler_error == NULL)
    {
        return;
    }

    safe_free(compiler_error->file_path);
    safe_free(compiler_error->message);
    safe_free(compiler_error);
}

static compiler_error* create_compiler_error(char* file_path, int line_number, int column, char* message, error_type type)
{
    /* handle allocation failing */
    compiler_error* new_compiler_error = (compiler_error*)safe_malloc(sizeof(compiler_error));
    if (new_compiler_error == NULL)
    {
        return NULL;
    }

    new_compiler_error->file_path = duplicate_string(file_path);
    new_compiler_error->line_number = line_number;
    new_compiler_error->column = column;
    new_compiler_error->message = duplicate_string(message);
    new_compiler_error->type = type;

    /* handle duplicate_string failing */
    if(new_compiler_error->file_path == NULL || new_compiler_error->message == NULL)
    {
        free_compiler_error(new_compiler_error);
        return NULL;
    }

    return new_compiler_error;
}

static void free_variable(variable* variable)
{
    if(variable == NULL)
    {
        return;
    }

    safe_free(variable->type);
    safe_free(variable->name);
    safe_free(variable); 
}

static void free_stackframe(stackframe* stackframe)
{
    if(stackframe == NULL)
    {
        return;
    }

    variable* current_var = stackframe->variables;
    while(current_var != NULL)
    {
        variable* next_var = current_var->next;
        free_variable(current_var);
        current_var = next_var;
    }

    safe_free(stackframe->caller_name);
    safe_free(stackframe->file_path);
    safe_free(stackframe);
}

static variable* duplicate_variable(variable* var)
{
    /* handle NULL variable */
    if(var == NULL)
    {
        return NULL;
    }

    /* handle allocation failing */
    variable* new_var = (variable*)safe_malloc(sizeof(variable));
    if(new_var == NULL)
    {
        return NULL;
    }

    new_var->type = duplicate_string(var->type);
    new_var->name = duplicate_string(var->name);
    new_var->value = var->value;
    new_var->next = NULL;

    /* handle duplicate_string failing */
    if(new_var->type == NULL || new_var->name == NULL)
    {
        free_variable(new_var);
        return NULL;
    }

    return new_var;
}

static bool move_state_variable_to_previous_stack(lsharp_state* state, stackframe* new_frame)
{
    variable* current_var = state->variables;
    variable* last_var = NULL;
    while(current_var != NULL)
    {
        /* handle duplicate_variable failing */
        variable* new_var = duplicate_variable(current_var);
        if(new_var == NULL)
        {
            free_stackframe(new_frame);
            return false;
        }

        new_frame->variables = new_var;
        if(new_frame->variables != NULL)
        {
            last_var->next = new_var;
        }

        last_var = new_var;
        current_var = current_var->next;
    }

    return true;
}

lsharp_state* create_lsharp_state()
{
    /* handle allocation failing */
    lsharp_state* state = (lsharp_state*)safe_malloc(sizeof(lsharp_state));
    if (state == NULL)
    {
        return NULL;
    }

    state->variables = NULL;
    state->stack = NULL;
    state->errors = NULL;

    /* TODO: initialize contracts, usertypes, etc. */

    return state;
}

void reset_lsharp_state(lsharp_state* state)
{
    if (state == NULL)
    {
        return;
    }

    if (state->stack != NULL)
    {
        safe_free_collection((void**)state->stack->variables);
    }

    safe_free_collection((void**)state->stack);
    safe_free_collection((void**)state->variables);
    safe_free_collection((void**)state->errors);
}

bool add_variable(lsharp_state* state, char* variable_type, char* variable_name, void* variable_value)
{
    /* handle allocation failing */
    variable* new_var = (variable*)safe_malloc(sizeof(variable));
    if (new_var == NULL)
    {
        return false;
    }

    new_var->type = duplicate_string(variable_type);
    new_var->name = duplicate_string(variable_name);
    new_var->value = variable_value;
    new_var->next = state->variables;

    /* handle duplicate_string failing */
    if(new_var->type == NULL || new_var->name == NULL)
    {
        free_variable(new_var);
        return false;
    }

    state->variables = new_var;
    return true;
}

variable* get_variable(lsharp_state* state, char* variable_name)
{
    variable* current = state->variables;
    while (current != NULL)
    {
        if (strcmp(current->name, variable_name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool add_stackframe(lsharp_state* state, char* caller_name, int line_number, char* file_path)
{
    /* handle allocation failing */
    stackframe* new_frame = (stackframe*)safe_malloc(sizeof(stackframe));
    if (new_frame == NULL)
    {
        return false;
    }

    new_frame->caller_name = duplicate_string(caller_name);
    new_frame->line_number = line_number;
    new_frame->file_path = duplicate_string(file_path);
    new_frame->variables = state->variables;
    new_frame->previous = state->stack;

    /* handle duplicate_string failing */
    if(new_frame->caller_name == NULL || new_frame->file_path == NULL)
    {
        free_stackframe(new_frame);
        return false;
    }

    /* duplicate the current stack's variables and put them into the previous stack */
    if (move_state_variable_to_previous_stack(state, new_frame) == false)
    {
        free_stackframe(new_frame);
        return false;
    }

    state->stack = new_frame;
    return true;
}

char* get_current_frame_file_and_line(lsharp_state* state)
{
    /* handle the first stack frame being current or file_path being NULL */
    if (state->stack == NULL || state->stack->file_path == NULL)
    {
        return NULL;
    }

    /* adjust size as needed */
    char* result = (char*)safe_malloc(strlen(state->stack->file_path) + 20);
    if (result == NULL)
    {
        return NULL;
    }

    sprintf(result, "%s:%d", state->stack->file_path, state->stack->line_number);
    return result;
}

void add_compiler_error(lsharp_state* state, char* file_path, int line_number, int column, char* message, error_type type)
{
    /* create a new compiler error to add to the front of the list */
    compiler_error* new_compiler_error = create_compiler_error(file_path, line_number, column, message, type);

    /* add `error` to the front of the list */
    new_compiler_error->next = state->errors;
    state->errors = new_compiler_error;
}

void report_compiler_error(lsharp_state* state)
{
    compiler_error* current_error = state->errors;
    while(current_error != NULL)
    {
        printf("Error (%s): %s at %s:%d:%d\n",
            current_error->type == ERROR_LEXICAL ? "Lexical" :
            current_error->type == ERROR_SYNTAX ? "Syntax" :
            current_error->type == ERROR_SEMANTIC ? "Semantic" : "Unknown",
            current_error->message,
            current_error->file_path ? current_error->file_path : "Unknown File",
            current_error->line_number,
            current_error->column
        );
        current_error = current_error->next;
    }
}