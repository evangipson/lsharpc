#include "main.h"

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    char* file_content = read_file(argv[1]);
    assert(file_content != NULL && "Unable to read L# file.");

    char** lexemes = get_lexemes(file_content);
    assert(lexemes != NULL && "Unable to parse L# file.");

    return 1;
}