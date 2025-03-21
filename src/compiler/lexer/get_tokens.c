#include "get_tokens.h"

static const char* keywords[] =
{
    "grab",
    "contract",
    "is",
    "return",
    "number",
    "text",
    "bit",
    "numbers",
    "texts",
    "bits",
    "on",
    "off",
    /* null-terminate array for looping */
    NULL
};

static bool is_keyword(char* identifier)
{
    if (identifier == NULL)
    {
        return false;
    }

    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(identifier, keywords[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

static char* get_identifier(char* source, int* position)
{
    int start_pos = *position;
    int length = 0;

    /* consume identifier characters */
    while (isalnum(source[*position]) || source[*position] == '_' || source[*position] == '-')
    {
        (*position)++;
        length++;
    }

    /* allocate memory for the identifier string with null terminator */
    char* identifier = (char*)safe_malloc((length + 1) * sizeof(char));
    if (identifier == NULL)
    {
        log_error("Compiler error: Failed to get identifier, could not allocate %zu bytes of memory.", (length + 1) * sizeof(char));
        return NULL;
    }

    /* Copy the identifier characters and add null-terminator */
    memcpy(identifier, &source[start_pos], length);
    if(identifier == NULL)
    {
        log_error("Compiler error: Failed to create identifier from source.");
        return NULL;
    }
    identifier[length] = '\0';

    return identifier;
}

static char* get_string_literal(char* source, int* position)
{
    /* consume the opening quote (') */
    (*position)++;
    int start_pos = *position;
    int length = 0;

    /* consume characters until the closing quote */
    while (source[*position] != '\'' && source[*position] != '\0')
    {
        (*position)++;
        length++;
    }

    /* guard against unterminated string literal */
    if (source[*position] != '\'')
    {
        log_error("Compiler error: Could not create string literal, unterminated string.");
        return NULL;
    }

    /* allocate memory for the string literal with null-terminator */
    char* string = (char*)malloc((length + 1) * sizeof(char));
    if (string == NULL)
    {
        log_error("Compiler error: Failed to get string literal, could not allocate %zu bytes of memory.", (length + 1) * sizeof(char));
        return NULL;
    }

    /* copy the string literal characters with null-terminator */
    memcpy(string, &source[start_pos], length);
    if(string == NULL)
    {
        log_error("Compiler error: Failed to create string literal from source.");
        return NULL;
    }
    string[length] = '\0';

    /* consume the closing quote (') */
    (*position)++;

    return string;
}

static token* get_next_token(char* source, int* position, int* line, int* column)
{
    /* check for end of file at the very beginning */
    if (source[*position] == '\0')
    {
        return create_token(TOKEN_END_OF_FILE, NULL, *line, *column);
    }

    /* loop to handle skipping comments and whitespace, to avoid recursion */
    while (true)
    {
        /* skip whitespace */
        while (isspace(source[*position]))
        {
            if (source[*position] == '\n')
            {
                (*line)++;
                (*column) = 1;
                (*position)++;
                continue;
            }
            (*column)++;
            (*position)++;
        }

        /* check for multi-line comments */
        if (source[*position] == '/' && source[*position + 1] == '*')
        {
            /* multi-line comment found, skip beginning comment indicator */
            (*position) += 2;
            while (source[*position] != '\0' && !(source[*position] == '*' && source[*position + 1] == '/'))
            {
                if (source[*position] == '\n')
                {
                    (*line)++;
                    (*column) = 1;
                    (*position)++;
                    continue;
                }
                (*column)++;
                (*position)++;
            }

            /* skip ending comment indicator */
            if (source[*position] == '*' && source[*position + 1] == '/')
            {
                (*position) += 2;
            }
            else
            {
                /* guard for unterminated comment */
                return create_token(TOKEN_ERROR, "Unterminated comment", *line, *column);
            }

            /* skip the rest of the tokenizing code and go to the beginning of the while loop */
            continue;
        }

        /* no comment or whitespace, exit the while loop */
        break;
    }

    /* check for end of file again, in case it was reached during comment or whitespace */
    if (source[*position] == '\0') {
        return create_token(TOKEN_END_OF_FILE, NULL, *line, *column);
    }

    /* check for keywords and identifiers */
    if (isalpha(source[*position]))
    {
        char* identifier = get_identifier(source, position);
        if (identifier == NULL)
        {
            return create_token(TOKEN_ERROR, NULL, *line, *column);
        }

        if (is_keyword(identifier))
        {
            token_type keyword_type = TOKEN_ERROR;

            /* TODO: create struct with char* and token_type for each token to shorten this */
            if (strcmp(identifier, "grab") == 0) keyword_type = TOKEN_KEYWORD_GRAB;
            else if (strcmp(identifier, "contract") == 0) keyword_type = TOKEN_KEYWORD_CONTRACT;
            else if (strcmp(identifier, "is") == 0) keyword_type = TOKEN_KEYWORD_IS;
            else if (strcmp(identifier, "return") == 0) keyword_type = TOKEN_KEYWORD_RETURN;
            else if (strcmp(identifier, "number") == 0) keyword_type = TOKEN_TYPE_NUMBER;
            else if (strcmp(identifier, "text") == 0) keyword_type = TOKEN_TYPE_TEXT;
            else if (strcmp(identifier, "bit") == 0) keyword_type = TOKEN_TYPE_BIT;
            else if (strcmp(identifier, "numbers") == 0) keyword_type = TOKEN_TYPE_NUMBERS;
            else if (strcmp(identifier, "texts") == 0) keyword_type = TOKEN_TYPE_TEXTS;
            else if (strcmp(identifier, "bits") == 0) keyword_type = TOKEN_TYPE_BITS;
            else if (strcmp(identifier, "on") == 0) keyword_type = TOKEN_BIT_LITERAL_ON;
            else if (strcmp(identifier, "off") == 0) keyword_type = TOKEN_BIT_LITERAL_OFF;

            token* token = create_token(keyword_type, identifier, *line, *column);

            safe_free(identifier);

            return token;
        }

        /* if it wasn't a keyword, it has to be an identifier */
        token* token = create_token(TOKEN_IDENTIFIER, identifier, *line, *column);

        safe_free(identifier);

        return token;
    }

    /* check for number literals and handle decimal point interpretation */
    if (isdigit(source[*position]) || source[*position] == '.')
    {
        int start_pos = *position;
        int length = 0;
        bool is_number = false;
        bool has_decimal = false;

        /* consume digits and optional decimal point */
        while (isdigit(source[*position]) || source[*position] == '.')
        {
            if (source[*position] != '.')
            {
                /* mark as number if digit found */
                is_number = true;
                (*position)++;
                length++;
                continue;
            }

            if (has_decimal)
            {
                /* multiple decimal points, error */
                return create_token(TOKEN_ERROR, "Invalid number format", *line, *column);
            }

            /* mark as decimal if decimal found */
            has_decimal = true;
            (*position)++;
            length++;
        }

        /* check if it's a number or just a period */
        if (is_number)
        {
            char* number = (char*)safe_malloc((length + 1) * sizeof(char));
            memcpy(number, &source[start_pos], length);
            number[length] = '\0';
            return create_token(TOKEN_NUMBER_LITERAL, number, *line, *column);
        }

        /* it's just a period, correct position and send back punctuation */
        (*position) = start_pos + 1;
        return create_token(TOKEN_PUNCTUATION_PERIOD, ".", *line, *column);
    }

    /* check for string literals */
    if (source[*position] == '\'')
    {
        char* string = get_string_literal(source, position);
        if (string == NULL)
        {
            return create_token(TOKEN_ERROR, NULL, *line, *column);
        }

        token* token = create_token(TOKEN_STRING_LITERAL, string, *line, *column);

        safe_free(string);

        return token;
    }

    /* check for template string literals */
    if (source[*position] == '`')
    {
        /**
         * TODO: extract template string literal.
         * this is more complex and will require more logic, including parsing the string
         * and any variables inside of it. for now, just return a token for the start.
         */
        (*position)++;
        return create_token(TOKEN_STRING_TEMPLATE_START, "`", *line, *column);
    }

    /* check for operators and punctuation */
    switch (source[*position])
    {
        case '=':
            (*position)++;
            return create_token(TOKEN_OPERATOR_ASSIGN, "=", *line, *column);
        case '+':
            (*position)++;
            return create_token(TOKEN_OPERATOR_PLUS, "+", *line, *column);
        case '-':
            (*position)++;
            return create_token(TOKEN_OPERATOR_MINUS, "-", *line, *column);
        case '*':
            (*position)++;
            return create_token(TOKEN_OPERATOR_MULTIPLY, "*", *line, *column);
        case '/':
            (*position)++;
            return create_token(TOKEN_OPERATOR_DIVIDE, "/", *line, *column);
        case '.':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_PERIOD, ".", *line, *column);
        case ',':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_COMMA, ",", *line, *column);
        case '(':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_OPEN_PAREN, "(", *line, *column);
        case ')':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_CLOSE_PAREN, ")", *line, *column);
        case '[':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_OPEN_BRACKET, "[", *line, *column);
        case ']':
            (*position)++;
            return create_token(TOKEN_PUNCTUATION_CLOSE_BRACKET, "]", *line, *column);
        default:
            (*position)++;
            return create_token(TOKEN_ERROR, NULL, *line, *column);
    }
}

token** get_tokens(char* source, size_t* token_count)
{
    token** tokens = NULL;
    size_t token_amount = 0;
    size_t tokens_capacity = 10;
    int position = 0;
    int line = 1;
    int column = 1;

    /* allocate enough memory to load in "chunks" of tokens */
    tokens = (token**)malloc(tokens_capacity * sizeof(token*));
    if (tokens == NULL)
    {
        log_error("Compiler error: Failed to allocate memory for tokens.");
        *token_count = token_amount;
        return NULL;
    }

    token* current_token = get_next_token(source, &position, &line, &column);
    while (current_token != NULL && current_token->type != TOKEN_END_OF_FILE)
    {
        /* if the buffer isn't full, consume a token at a time */
        if (token_amount < tokens_capacity)
        {
            tokens[token_amount] = current_token;
            token_amount++;
            current_token = get_next_token(source, &position, &line, &column);
            continue;
        }

        /* if our tokens buffer is full, reallocate */
        tokens_capacity = tokens_capacity * 2;
        token** temp_tokens = (token**)realloc(tokens, tokens_capacity * sizeof(token*));
        if (temp_tokens == NULL)
        {
            log_error("Compiler error: Failed to reallocate memory for tokens.");
            free_tokens(tokens);
            *token_count = token_amount;
            return NULL;
        }
        tokens = temp_tokens;

        /* consume the next token after reallocating */
        tokens[token_amount] = current_token;
        token_amount++;
        current_token = get_next_token(source, &position, &line, &column);
    }

    /* add a null-terminator */
    tokens = (token**)realloc(tokens, (token_amount + 1) * sizeof(token*));
    tokens[token_amount] = NULL;

    *token_count = token_amount;
    return tokens;
}