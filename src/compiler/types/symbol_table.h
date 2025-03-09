#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../extensions/memory_extensions.h"
#include "../extensions/string_extensions.h"

typedef enum symbol_type {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    /* TODO: other symbol types */
} symbol_type;

typedef struct symbol symbol;

typedef struct symbol_table symbol_table;

struct symbol {
    char* name;
    symbol_type type;
    /// @brief Index in the VM's variables or functions array
    int index;
    /// @brief For linked lists in a hash table
    struct symbol* next;
};

struct symbol_table {
    /// @brief Array of linked lists (or a hash table).
    symbol** table;
    /// @brief Size of the table.
    int size;
};

symbol_table* create_symbol_table(int size);

unsigned int hash_symbol(const char* name, int size);

bool insert_symbol(symbol_table* table, const char* name, symbol_type type, int index);

symbol* lookup_symbol(symbol_table* table, const char* name);

bool free_symbol_table(symbol_table* table);

#endif