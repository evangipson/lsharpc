#include "get_file_content.h"

static long get_file_length(FILE* file_pointer)
{
    assert(file_pointer != NULL && "[file]: get_file_length was provided a null file_pointer.");

    fseek(file_pointer, 0, SEEK_END);
    long file_size = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    return file_size;
}

const char* get_file_content(const char* file_name)
{
    log_info("[get_file_content]: getting file content for %s", file_name);

    FILE* file_pointer = fopen(file_name, "rb");
    long file_length = get_file_length(file_pointer);
    log_info("[get_file_content]: file length of %s: %ld", file_name, file_length);

    return "";
}