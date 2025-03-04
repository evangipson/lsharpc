#include "read_file.h"

static size_t get_file_length(FILE* file_pointer)
{
    assert(file_pointer != NULL && "[file]: get_file_length was provided a null file_pointer.");

    fseek(file_pointer, 0, SEEK_END);
    long file_size = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    return (size_t)file_size;
}

char* read_file(const char* file_path)
{
    FILE* file_pointer = fopen(file_path, "rb");
    assert(file_pointer != NULL && "[file]: allocate_file_to_buffer was not able to open file.");

    size_t file_length = get_file_length(file_pointer);
    /* allocate enough memory in the buffer and the null terminator */
    char* file_contents = (char*)safe_malloc(file_length + 1);
    assert(file_contents != NULL && "[file]: allocate_file_to_buffer was not able to allocate the memory to read file.");

    size_t bytes_read = fread(file_contents, 1, file_length, file_pointer);
    assert(bytes_read == file_length && "[file]: allocate_file_to_buffer was not able to read file.");

    file_contents[bytes_read] = '\0';
    fclose(file_pointer);

    return file_contents;
}