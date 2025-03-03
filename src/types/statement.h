#ifndef STATEMENT
#define STATEMENT

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    unsigned int line_number;
    char* file_path;
    char* type;
    char* action;
    char* name;
    bool (*expression)();
} statement;

statement create_statement(unsigned int line_number, char* file_path, char* type, char* action, char* name, bool (*expression)());

#endif