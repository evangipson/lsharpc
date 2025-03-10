#include "main.h"

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        log_info("Usage: vm <bytecode_file>");
        return 1;
    }

    virtual_machine vm;
    create_virtual_machine(&vm);
    if (load_bytecode_from_file(&vm, argv[1]) == false)
    {
        log_info("Failed to load \"%s\" bytecode file.", argv[1]);
        return 1;
    }
    if(run_vm(&vm) == false)
    {
        log_error("Failed to run \"%s\" bytecode.", argv[1]);
        return 1;
    }
    log_info("Ran \"%s\" bytecode file.", argv[1]);

    safe_free(&vm);

    return 0;
}