#include "memory_extensions.h"

void* safe_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed (size: %zu)\n", size);
        /* TODO: use add_compiler_error and take in lsharp_state* state instead of exiting. */
        exit(1);
    }

    return ptr;
}

void safe_free(void* ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
    }
}

void safe_free_collection(void** collection)
{
    if (collection == NULL)
    {
        return;
    }

    for (size_t i = 0; collection[i] != NULL; i++)
    {
        safe_free(collection[i]);
    }
    safe_free(collection);
}