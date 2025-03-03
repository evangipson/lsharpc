#include "string_extensions.h"

char* duplicate_string(const char* source)
{
    if (source == NULL)
    {
        return NULL;
    }

    size_t length = strlen(source);

    /* +1 to allocated memory size for null terminator */
    char* destination = (char*)safe_malloc((length + 1) * sizeof(char));
    if (destination == NULL)
    {
        return NULL;
    }

    strcpy(destination, source);

    return destination;
}