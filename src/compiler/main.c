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

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    log_info("[lsharpc]: attempting to read file \"%s\"", argv[1]);
    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    log_info("[lsharpc]: got content from L# source file \"%s\", running lexer", argv[1]);
    
    token** tokens = get_tokens(file_content);
    print_tokens(tokens);

    free_tokens(tokens);

    return 1;
}