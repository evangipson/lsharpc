#ifndef LOGGER
#define LOGGER
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

/// @brief Logs a debug message to the console.
/// @param message The message to log.
void log_debug(const char* message, ...);

/// @brief Logs an error message to the console.
/// @param message The message to log.
void log_error(const char* message, ...);

/// @brief Logs an informational message to the console.
/// @param message The message to log.
void log_info(const char* message, ...);

/// @brief Logs a warning message to the console.
/// @param message The message to log.
void log_warning(const char* message, ...);