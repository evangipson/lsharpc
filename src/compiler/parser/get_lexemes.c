#include "get_lexemes.h"

static size_t get_lines(char* file_content, char*** file_lines)
{
    size_t line_count = 0;
    char* line = strtok(file_content, "\n");
    while(line != NULL)
    {
        line = strtok(NULL, "\n");
        ++line_count;
    }

    *file_lines = (char**)safe_malloc(line_count * (sizeof(char*)));
    assert(*file_lines != NULL && "[parser]: get_lines unable to allocate memory for reading file's lines.");

    (*file_lines)[0] = strtok(file_content, "\n");
    for(size_t i = 1; i < line_count; i++)
    {
        (*file_lines)[i] = line;
        line = strtok(NULL, "\n");
    }

    assert(line_count > 0 && "[parser]: provided file content with 0 lines.");
    return line_count;
}

lexeme** get_lexemes(char* file_content)
{
    char** file_lines = NULL;
    size_t line_count = get_lines(file_content, &file_lines);
    log_debug("[parser]: get_lexemes got %d lines from file_content", line_count);

    lexeme** lexemes = (lexeme**)safe_malloc(line_count * (sizeof(lexeme)));
    for(size_t i = 0; i < line_count; i++)
    {
        /* TODO: parse file_lines and get lexemes out of them */
        lexemes[i] = create_lexeme(NULL, NULL, i);
    }
    free(file_lines);

    return lexemes;
}