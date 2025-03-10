#include "symbol_table.h"

symbol_table* create_symbol_table(int size)
{
    symbol_table* table = (symbol_table*)safe_malloc(sizeof(symbol_table));
    table->size = size;
    /* initialize to NULL */
    table->table = (symbol**)calloc(size, sizeof(symbol*));
    return table;
}

unsigned int hash_symbol(const char* name, int size)
{
    unsigned int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
    {
        hash = hash * 31 + name[i];
    }
    return hash % size;   
}

bool insert_symbol(symbol_table* table, const char* name, symbol_type type, int index)
{
    unsigned int hash_index = hash_symbol(name, table->size);
    symbol* new_symbol = (symbol*)safe_malloc(sizeof(symbol));
    new_symbol->name = duplicate_string(name);
    new_symbol->type = type;
    new_symbol->index = index;
    new_symbol->next = table->table[hash_index];
    table->table[hash_index] = new_symbol;

    return true;
}

symbol* lookup_symbol(symbol_table* table, const char* name)
{
    unsigned int hash_index = hash_symbol(name, table->size);
    symbol* current = table->table[hash_index];
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

bool free_symbol_table(symbol_table* table)
{
    for (int i = 0; i < table->size; i++)
    {
        symbol* current = table->table[i];
        while (current != NULL)
        {
            symbol* next = current->next;
            safe_free(current->name);
            safe_free(current);
            current = next;
        }
    }
    safe_free(table->table);
    safe_free(table);

    return true;
}