#include "string_extensions.h"

char* duplicate_string(const char* source)
{
    if (source == NULL)
    {
        return NULL;
    }

    // Check for null terminator (basic check)
    if (strchr(source, '\0') == NULL)
    {
        log_error("[duplicate_string]: input string is not null-terminated.");
        return NULL;
    }

    /* get length of string, minus the null terminator */
    size_t length = strlen(source);

    /* allocated memory and add one for null terminator */
    char* destination = (char*)safe_malloc(length + 1);
    if (destination == NULL)
    {
        log_error("[duplicate_string]: could not allocate memory for destination string.");
        return NULL;
    }

    /* copy all bytes and force a null terminator */
    memcpy(destination, source, length);
    destination[length] = '\0';

    return destination;
}