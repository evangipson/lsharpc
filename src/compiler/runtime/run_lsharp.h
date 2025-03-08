#ifndef RUN_LSHARP
#define RUN_LSHARP
#include "../types/abstract_syntax_tree.h"
#include "../types/lsharp_state.h"

/// @brief Runs an L# program using the `abstract_syntax_tree`.
/// @param abstract_syntax_tree A tree containing all the parsed tokens from an L# source file.
void run_lsharp(abstract_syntax_node* abstract_syntax_tree);

#endif