#ifndef MAIN
#define MAIN
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "virtual_machine/virtual_machine.h"

/// @brief The main entry point of the L# runtime.
/// @param argc The number of command-line arguments given to the runtime.
/// @param argv An array of strings provided on the command-line to the runtime.
int main(int argc, const char* argv[]);

#endif