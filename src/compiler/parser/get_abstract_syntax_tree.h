#ifndef GET_ABSTRACT_SYNTAX_TREE
#define GET_ABSTRACT_SYNTAX_TREE
#include <stdio.h>
#include <stdlib.h>
#include "../../core/extensions/memory_extensions.h"
#include "../../core/logger/logger.h"
#include "../lexer/get_tokens.h"
#include "../types/abstract_syntax_tree.h"

abstract_syntax_node* get_abstract_syntax_tree(token** tokens, size_t number_of_tokens);

void free_ast(abstract_syntax_node* root);

#endif