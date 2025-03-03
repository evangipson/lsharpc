#include "main.h"

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    char* file_content;
    allocate_file_to_buffer(argv[1], &file_content);
    assert(file_content != NULL && "Unable to read L# file.");
    free(file_content);

    return 1;
}