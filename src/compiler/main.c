#include "main.h"

int main(int argc, const char* argv[])
{
    if (argv[1] == NULL)
    {
        log_error("Compiler error: An L# entry file must be provided as an argument to the L# compiler.");
        return 0;
    }
    
    if (strlen(argv[1]) <= 0)
    {
        log_error("Compiler error: A valid L# entry file must be provided as an argument to the L# compiler.");
        return 0;
    }

    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    size_t token_count = 0;
    token** tokens = get_tokens(file_content, &token_count);
    if (tokens == NULL)
    {
        log_error("Compiler error: Failed to get tokens from L# source file.");
        return 0;
    }

    abstract_syntax_node* abstract_syntax_tree = get_abstract_syntax_tree(tokens, token_count);
    if (abstract_syntax_tree == NULL)
    {
        log_error("Compiler error: Failed to get abstract syntax tree from tokens.");
        return 0;
    }
    free_tokens(tokens);

    int instruction_count;
    instruction* instructions = compile_ast_to_bytecode(abstract_syntax_tree, &instruction_count, "bin/program.lbc");

    log_info("L# compilation complete.");

    free_ast(abstract_syntax_tree);
    safe_free_collection((void**)instructions);

    return 1;
}