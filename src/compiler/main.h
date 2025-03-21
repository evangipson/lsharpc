#ifndef LSHARPC
#define LSHARPC
#include <assert.h>
#include <string.h>
#include "../core/logger/logger.h"
#include "file/read_file.h"
#include "lexer/get_tokens.h"
#include "parser/get_abstract_syntax_tree.h"
#include "bytecode_generator/bytecode_generator.h"

/// @brief The main entry point of the L# compiler.
/// @param argc The number of command-line arguments given to the compiler.
/// @param argv An array of strings provided on the command-line to the compiler.
int main(int argc, const char* argv[]);

#endif