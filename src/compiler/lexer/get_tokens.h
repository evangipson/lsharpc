#ifndef GET_TOKENS
#define GET_TOKENS
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../core/extensions/memory_extensions.h"
#include "../../core/extensions/string_extensions.h"
#include "../../core/logger/logger.h"
#include "../types/token.h"

/// @brief Gets all tokens from the L# `source`.
/// @param source The L# source code to get tokens from.
/// @param token_count The count of total tokens from `source`.
/// @return A collection of tokens, or `NULL` if tokenization is unsuccessful.
token** get_tokens(char* source, size_t* token_count);

#endif