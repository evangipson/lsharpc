#include "statement.h"

statement create_statement(unsigned int line_number, char* file_path, char* type, char* action, char* name, bool (*expression)())
{
    statement new_statement = { line_number, file_path, type, action, name, expression };
    return new_statement;
}