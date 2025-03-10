#include "memory_extensions.h"

void* safe_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        log_error("[safe_malloc]: Memory allocation failed (size: %zu bytes)", size);
        exit(-1);
    }

    return ptr;
}

void safe_free(void* ptr)
{
    if (ptr == NULL)
    {
        log_error("[safe_free]: provided pointer was already null");
        return;
    }

    free(ptr);
}

void safe_free_collection(void** collection)
{
    if (collection == NULL)
    {
        log_error("[safe_free_collection]: provided collection was null");
        return;
    }

    /* free all elements of the collection */
    size_t index = 0;
    while (collection[index] != NULL)
    {
        safe_free(collection[index]);
        index++;
    }

    /* free the collection itself */
    safe_free(collection);
}