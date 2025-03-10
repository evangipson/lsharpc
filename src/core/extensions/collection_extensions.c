#include "collection_extensions.h"

/// @brief A simple callback function to copy a string, used in `copy_strings`.
/// @param string_pointer A pointer to a string to copy.
/// @return A copy of the string at `string_pointer` with newly-allocated memory.
static void* copy_string(const void* string_pointer)
{
    return (void*)duplicate_string((const char*)string_pointer);
}

size_t count_collection(void** collection)
{
    /* safeguard against null collection */
    if (collection == NULL)
    {
        return 0;
    }

    /* count until a null terminated entry is found */
    size_t count = 0;
    while (collection[count] != NULL)
    {
        count++;
    }

    return count;
}

void** copy_collection(void** source_collection, copy_function callback)
{
    /* safeguard against null collection */
    if (source_collection == NULL)
    {
        log_error("[copy_collection]: unable to copy collection, source collection was null");
        return NULL;
    }

    /* safeguard against null callback function */
    if (callback == NULL)
    {
        log_error("[copy_collection]: unable to copy collection, callback copy function was null");
        return NULL;
    }

    /* allocate enough for the collection to include a null terminator */
    size_t count = count_collection(source_collection);
    void** destination_collection = (void**)safe_malloc((count + 1) * sizeof(void*));
    /* safeguard against safe_malloc failure */
    if (destination_collection == NULL)
    {
        return NULL;
    }

    /* run the callback to copy each member */
    for (size_t i = 0; i < count; i++)
    {
        destination_collection[i] = callback(source_collection[i]);
        /* handle callback failure */
        if(destination_collection[i] == NULL)
        {
            for(size_t j = 0; j < i; j++)
            {
                safe_free(destination_collection[j]);
            }
            safe_free(destination_collection);

            log_error("[copy_collection]: unable to copy collection, problem with element %zu", i + 1);
            return NULL;
        }
    }

    /* add null terminator so safe_free_collection can be used */
    destination_collection[count] = NULL;

    return destination_collection;
}

char** copy_strings(char** source_strings)
{
    return (char**)copy_collection((void**)source_strings, copy_string);
}