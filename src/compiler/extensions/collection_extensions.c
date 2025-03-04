#include "collection_extensions.h"

size_t count_collection(void** collection)
{
    /* safeguard against null collection */
    if (collection == NULL)
    {
        return 0;
    }

    size_t count = 0;
    while (collection[count] != NULL)
    {
        count++;
    }

    return count;
}

void** copy_collection(void** source_collection)
{
    /* safeguard against null source_collection */
    if (source_collection == NULL)
    {
        return NULL;
    }

    /* get the size of the collection + 1 (for a null terminator) */
    size_t count = count_collection(source_collection);
    void** destination_collection = (void**)safe_malloc((count + 1) * sizeof(void*));

    if (destination_collection == NULL)
    {
        /* allocation failed */
        return NULL;
    }

    for (size_t i = 0; i < count; i++)
    {
        /* shallow copy. If the collection contains pointers to strings, you will need to use duplicate_string */
        destination_collection[i] = source_collection[i];
    }

    destination_collection[count] = NULL;
    return destination_collection;
}