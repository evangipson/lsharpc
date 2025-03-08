#include "get_lexemes.h"

static size_t get_lines(char* file_content, char*** file_lines)
{
    size_t line_count = 0;
    char* content_copy = duplicate_string(file_content);
    if (content_copy == NULL)
    {
        return 0;
    }

    char* write_ptr = content_copy;
    char* read_ptr = content_copy;
    while (*read_ptr != '\0')
    {
        /* handle CRLF files by skipping carriage returns */
        if (*read_ptr == '\r')
        {
            read_ptr++;
            continue;
        }

        *write_ptr = *read_ptr;
        write_ptr++;
        read_ptr++;
    }
    *write_ptr = '\0';

    char* current = content_copy;
    while (*current != '\0')
    {
        if (*current == '\n')
        {
            line_count++;
        }
        current++;
    }
    if (content_copy[strlen(content_copy) - 1] != '\n')
    {
        line_count++;
    }

    *file_lines = (char**)safe_malloc(line_count * sizeof(char*));
    assert(*file_lines != NULL && "[parser]: get_lines unable to allocate memory for reading file's lines.");

    current = content_copy;
    size_t line_index = 0;
    char* line_start = current;
    while (*current != '\0')
    {
        if (*current == '\n')
        {
            *current = '\0';
            (*file_lines)[line_index] = duplicate_string(line_start);
            line_index++;
            line_start = current + 1;
        }
        current++;
    }

    if (line_start < current)
    {
        (*file_lines)[line_index] = duplicate_string(line_start);
    }

    safe_free(content_copy);

    assert(line_count > 0 && "[parser]: provided file content with 0 lines.");
    return line_count;
}

static size_t get_words(char* line, char*** words)
{
    size_t word_count = 0;
    char* line_copy = duplicate_string(line);
    if (line_copy == NULL)
    {
        return 0;
    }

    char* current = line_copy;
    while (*current != '\0')
    {
        if (*current == ' ')
        {
            word_count++;
        }
        current++;
    }
    if(line_copy[strlen(line_copy) - 1] != ' ' && line_copy[strlen(line_copy) - 1] != '\t' && strlen(line_copy) > 0)
    {
        word_count++;
    }

    *words = (char**)safe_malloc(word_count * sizeof(char*));
    assert(*words != NULL && "[parser]: get_lines unable to allocate memory for reading words from a line.");

    current = line_copy;
    size_t word_index = 0;
    char* word_start = current;
    while (*current != '\0')
    {
        if (*current == ' ')
        {
            *current = '\0';
            (*words)[word_index] = duplicate_string(word_start);
            word_index++;
            word_start = current + 1;
        }
        current++;
    }

    if (word_start < current)
    {
        (*words)[word_index] = duplicate_string(word_start);
    }

    safe_free(line_copy);

    return word_count;
}

lexeme** get_lexemes(char* file_content)
{
    char** file_lines = NULL;
    size_t line_count = get_lines(file_content, &file_lines);
    log_debug("[parser]: get_lexemes got %d lines from file_content", line_count);

    lexeme** lexemes = (lexeme**)safe_malloc(line_count * (sizeof(lexeme)));
    if(lexemes == NULL)
    {
        return NULL;
    }

    for(size_t i = 0; i < line_count; i++)
    {
        log_debug("[parser]: processing line %zu", i + 1);

        char** words = NULL;

        char* line = duplicate_string(file_lines[i]);
        if(line == NULL)
        {
            log_error("[parser]: unable to duplicate line %d", i + 1);
            return NULL;
        }
        log_info("[parser]: get_lexemes line %d = \"%s\"", i + 1, line);

        size_t word_count = get_words(line, &words);
        log_info("[parser]: get_lexemes found %d words on line %d", word_count, i + 1);

        for(size_t n = 0; n < word_count; n++)
        {
            log_info("[parser]: word: %s", words[n]);
        }

        lexemes[i] = create_lexeme(words, NULL, i);

        safe_free(line);
        safe_free_collection((void**)words);
    }
    safe_free_collection((void**)file_lines);

    return lexemes;
}