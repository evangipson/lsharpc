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

static void log_message(LOG_CONTEXT context, const char* message, va_list args)
{
    assert(message != NULL && "[logger]: validate_message provided a null message.");
    assert(strlen(message) > 0 && "[logger]: validate_message provided an empty message.");

    char formatted_message[256];
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);

    printf("%s[%s]" RESET_ESCAPE_CODE ":\n", context.color_code, context.name);
    printf("   %s\n", formatted_message);
}

void log_debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log_message(log_contexts[0], message, args);
    va_end(args);
}

void log_error(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log_message(log_contexts[1], message, args);
    va_end(args);
}

void log_info(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log_message(log_contexts[2], message, args);
    va_end(args);
}

void log_warning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log_message(log_contexts[3], message, args);
    va_end(args);
}