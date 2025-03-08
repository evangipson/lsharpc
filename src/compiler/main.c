#include "main.h"

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    log_info("[lsharpc]: attempting to read file \"%s\"", argv[1]);
    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    log_info("[lsharpc]: got content from file \"%s\", attempting to parse into lexemes", argv[1]);
    lexeme** lexemes = get_lexemes(file_content);
    assert(lexemes != NULL && "Unable to parse L# file.");

    size_t lexeme_count = count_collection((void**)lexemes);
    log_info("[lsharpc]: got %zu lexemes from file content", lexeme_count);

    return 1;
}