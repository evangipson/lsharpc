#include "logger.h"

#define CYAN_ESCAPE_CODE    "\x1B[36m"
#define GREEN_ESCAPE_CODE   "\x1B[32m"
#define RED_ESCAPE_CODE     "\x1B[31m"
#define RESET_ESCAPE_CODE   "\x1B[0m"
#define YELLOW_ESCAPE_CODE  "\x1B[33m"

typedef enum
{
    LOG_SEVERITY_UNKNOWN,
    LOG_SEVERITY_DEBUG,
    LOG_SEVERITY_ERROR,
    LOG_SEVERITY_INFORMATION,
    LOG_SEVERITY_WARNING,
} LOG_SEVERITY;

typedef struct
{
    const char* name;
    LOG_SEVERITY severity;
    const char* color_code;
} LOG_CONTEXT;

static const LOG_CONTEXT log_contexts[] =
{
    { "debug", LOG_SEVERITY_DEBUG, GREEN_ESCAPE_CODE },
    { "error", LOG_SEVERITY_ERROR, RED_ESCAPE_CODE },
    { "information", LOG_SEVERITY_INFORMATION, CYAN_ESCAPE_CODE },
    { "warning", LOG_SEVERITY_WARNING, YELLOW_ESCAPE_CODE },
};

static void log_message(LOG_CONTEXT context, size_t buffer_size,  const char* message, va_list args)
{
    assert(message != NULL && "[logger]: log_message provided a null message.");
    assert(strlen(message) > 0 && "[logger]: log_message provided an empty message.");

    /* purposefully using `malloc` here instead of `safe_malloc` so there is no circular dependency with memory_extensions.h */
    char* formatted_message = (char*)malloc(buffer_size + 1);
    assert(formatted_message != NULL && "[logger]: log_message unable to allocate buffer for message.");

    vsnprintf(formatted_message, buffer_size + 1, message, args);
    printf("%s[%s]" RESET_ESCAPE_CODE ":\n", context.color_code, context.name);
    printf("   %s\n", formatted_message);

    free(formatted_message);
}

static size_t get_buffer_size(const char* message, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    size_t buffer_size = vsnprintf(NULL, 0, message, args_copy);
    assert(buffer_size > 0 && "[logger]: get_buffer_size not able to determine buffer of message.");

    va_end(args_copy);
    return buffer_size;
}

void log_debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);

    size_t buffer_size = get_buffer_size(message, args);
    log_message(log_contexts[0], buffer_size, message, args);

    va_end(args);
}

void log_error(const char* message, ...)
{
    va_list args;
    va_start(args, message);

    size_t buffer_size = get_buffer_size(message, args);
    log_message(log_contexts[1], buffer_size, message, args);

    va_end(args);
}

void log_info(const char* message, ...)
{
    va_list args;
    va_start(args, message);

    size_t buffer_size = get_buffer_size(message, args);
    log_message(log_contexts[2], buffer_size, message, args);

    va_end(args);
}

void log_warning(const char* message, ...)
{
    va_list args;
    va_start(args, message);

    size_t buffer_size = get_buffer_size(message, args);
    log_message(log_contexts[3], buffer_size, message, args);

    va_end(args);
}