#include "get_lexemes.h"

static size_t get_lines(char* file_content, char*** file_lines)
{
    /* duplicate the file content to not alter memory of what is provided to the function */
    size_t line_count = 0;
    char* content_copy = duplicate_string(file_content);
    if (content_copy == NULL)
    {
        return 0;
    }

    /* count the lines in the content by searching for new lines */
    char* current = content_copy;
    while (*current != '\0')
    {
        if (*current == '\n')
        {
            line_count++;
        }
        current++;
    }

    /* add a line if the last line doesn't end with a newline */
    if (line_count > 0 && content_copy[strlen(content_copy) - 1] != '\n') {
        line_count++;
    }

    /* make sure there is at least 1 line in the file provided */
    assert(line_count > 0 && "[get_lines]: provided file content with 0 lines");

    /* allocate enough space for all the lines and a null terminator at the end */
    *file_lines = (char**)safe_malloc((line_count + 1) * sizeof(char*));
    assert(*file_lines != NULL && "[get_lines]: get_lines unable to allocate memory for reading file's lines");

    /* duplicate each line of the file contents */
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

    /* handle the last line (if any) */
    if (line_start < current)
    {
        (*file_lines)[line_index] = duplicate_string(line_start);
        line_index++;
    }

    /* manually null terminate file lines, safe_free_collection relies on a null terminator */
    (*file_lines)[line_index] = NULL;

    safe_free(content_copy);

    return line_count;
}

static size_t get_words(char* line, char*** words)
{
    /* guard against empty lines */
    if (line == NULL || strlen(line) == 0)
    {
        log_warning("[get_words]: empty line passed to get_words, returning 0 word count");
        return 0;
    }

    /* duplicate the line to not alter memory of what is provided to the function */
    size_t word_count = 0;
    char* line_copy = duplicate_string(line);
    /* guard against duplicate_string failure */
    if (line_copy == NULL)
    {
        return 0;
    }

    /* count the amount of words in the line */
    char* current = line_copy;
    while (*current != '\0')
    {
        if (*current == ' ' || *current == '\t' || *current == '\n' || *current == '\r')
        {
            word_count++;
        }
        current++;
    }

    /* count the last word of the line, no matter what "end of line" character is at the end */
    if(line_copy[strlen(line_copy) - 1] != ' '
        && line_copy[strlen(line_copy) - 1] != '\t'
        && line_copy[strlen(line_copy) - 1] != '\n'
        && line_copy[strlen(line_copy) - 1] != '\r'
        && strlen(line_copy) > 0)
    {
        word_count++;
    }

    /* allocate words with enough memory to accept a null terminator */
    *words = (char**)safe_malloc((word_count + 1) * sizeof(char*));
    assert(*words != NULL && "[get_words]: get_lines unable to allocate memory for reading words from a line");

    /* copy the current word into the words array */
    current = line_copy;
    size_t word_index = 0;
    char* word_start = current;
    while (*current != '\0')
    {
        if (*current == ' ' || *current == '\t' || *current == '\n' || *current == '\r')
        {
            *current = '\0';
            (*words)[word_index] = duplicate_string(word_start);
            word_index++;
            word_start = current + 1;
        }
        current++;
    }

    /* handle the last word ending without a line terminator */
    if (word_start != current)
    {
        (*words)[word_index] = duplicate_string(word_start);
        word_index++;
    }

    /* null-terminate the words array */
    (*words)[word_index] = NULL;

    safe_free(line_copy);
    safe_free(line);

    return word_count;
}

lexeme** get_lexemes(char* file_content)
{
    /* get the lines from file content */
    char** file_lines = NULL;
    size_t line_count = get_lines(file_content, &file_lines);

    /* allocate enough memory for an array of lexemes that will be null-terminated */
    lexeme** lexemes = (lexeme**)safe_malloc(line_count * (sizeof(lexeme*)));
    if(lexemes == NULL)
    {
        log_error("[get_lexemes]: failed to allocate memory for lexemes collection.");
        return NULL;
    }

    /* for every line, add a lexeme with tokens and/or expressions */
    char** words = NULL;
    for(size_t i = 0; i < line_count; i++)
    {
        words = NULL;
        char* line = duplicate_string(file_lines[i]);
        if(line == NULL)
        {
            log_error("[get_lexemes]: failed to duplicate the file lines string for line %zu", i + 1);
            return NULL;
        }

        size_t word_count = get_words(line, &words);
        if (word_count == 0)
        {
            continue;
        }

        lexemes[i] = create_lexeme(words, NULL, i);
    }

    /* null terminate the lexemes collection */
    lexemes[line_count] = NULL;

    /* free all unused memory */
    safe_free_collection((void**)words);
    safe_free_collection((void**)file_lines);
    safe_free((void*)file_content);

    return lexemes;
}