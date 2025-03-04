#include "statement.h"

statement* create_statement(unsigned int line_number, char* file_path, char* type, char* action, char* name, bool (*expression)())
{
    /* handle allocation failing */
    statement* new_statement = (statement*)safe_malloc(sizeof(statement));
    if (new_statement == NULL)
    {
        return NULL;
    }

    new_statement->line_number = line_number;
    new_statement->file_path = duplicate_string(file_path);
    new_statement->type = duplicate_string(type);
    new_statement->action = duplicate_string(action);
    new_statement->name = duplicate_string(name);
    new_statement->expression = expression;

    /* if any expected properties haven't been properly allocated, free the memory and return NULL */
    bool is_file_path_null = new_statement->file_path == NULL;
    bool is_type_null = new_statement->type == NULL;
    bool is_action_null = new_statement->action == NULL;
    bool is_name_null = new_statement->name == NULL;
    if (is_file_path_null || is_type_null || is_action_null || is_name_null)
    {
        free_statement(new_statement);
        return NULL;
    }

    return new_statement;
}

void free_statement(statement* statement)
{
    if (statement == NULL)
    {
        return;
    }

    safe_free(statement->file_path);
    safe_free(statement->type);
    safe_free(statement->action);
    safe_free(statement->name);
    safe_free(statement);
}