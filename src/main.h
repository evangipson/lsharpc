#ifndef LSHARPC
#define LSHARPC

#include <assert.h>
#include <string.h>
#include "debug/logger.h"
#include "file/read_file.h"
#include "parser/get_lexemes.h"

/// @brief The main entry point of the L# compiler.
/// @param argc The number of command-line arguments given to the compiler.
/// @param argv An array of strings provided on the command-line to the compiler.
int main(int argc, const char* argv[]);

#endif