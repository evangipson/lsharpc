#include "string_extensions.h"

char* duplicate_string(const char* source)
{
    if (source == NULL)
    {
        return NULL;
    }

    size_t length = strlen(source);

    /* +1 to malloc size for null terminator */
    char* destination = (char*)malloc((length + 1) * sizeof(char));
    if (destination == NULL)
    {
        return NULL;
    }

    strcpy(destination, source);

    return destination;
}