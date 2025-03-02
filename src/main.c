#include "main.h"

int main(int argc, const char* argv[])
{
    assert(argv[1] != NULL && "You must provide an L# entry file as an argument to the L# compiler.");
    assert(strlen(argv[1]) > 0 && "You must provide a valid L# entry file as an argument to the L# compiler.");

    const char* file_content = get_file_content(argv[1]);
    
    return 1;
}