#include "main.h"

static void print_tokens(token** tokens)
{
    if (tokens == NULL)
    {
        log_info("[lsharpc]: failed to get tokens from L# source file");
        return;
    }

    log_info("[lsharpc]: got tokens from L# source file");
    for (size_t i = 0; tokens[i] != NULL; i++)
    {
        printf("\ttoken %zu (line %d, column %d)\n", i, tokens[i]->line, tokens[i]->column);
        printf("\t  type  : %s\n", get_token_type_name(tokens[i]->type));
        printf("\t  value : %s\n", tokens[i]->value);
    }
}

static void print_ast(abstract_syntax_node* node)
{
    if (node == NULL)
    {
        log_info("[lsharpc]: failed to get abstract syntax tree from tokens");
        return;
    }

    log_info("[lsharpc]: got abstract syntax tree from tokens");
    print_ast_node(node, 0);
}

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    log_info("[lsharpc]: attempting to read file \"%s\"", argv[1]);
    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    log_info("[lsharpc]: got content from L# source file \"%s\", running lexer", argv[1]);
    
    size_t token_count = 0;
    token** tokens = get_tokens(file_content, &token_count);
    print_tokens(tokens);

    abstract_syntax_node* abstract_syntax_tree = get_abstract_syntax_tree(tokens, token_count);
    print_ast(abstract_syntax_tree);
    free_tokens(tokens);

    int instruction_count;
    instruction* instructions = compile_ast_to_bytecode(abstract_syntax_tree, &instruction_count, "program.lbc");
    log_info("[lsharpc]: compiled bytecode into file \"program.lbc\"");

    free_ast(abstract_syntax_tree);
    safe_free_collection((void**)instructions);

    return 1;
}