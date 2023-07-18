#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statementer.h"
#include "directive_handler.h"
#include "assembler.h"
#include "logger.h"
#include "string_utils.h"

#define VALID_DIRECTIVES ".data, .string, .entry, .extern"

// assume the structure of the line is : "OPTIONAL_LABEL: .<SOME_STRING>..."
void validate_directive(Assembler *assembler, Statement *statement)
{
    Directive *directive = new_directive(statement);
    if (directive->type == -1)
    {
        // fixme
        error("Found invalid directive %s, valid directives are %s", directive->type_str, VALID_DIRECTIVES);
    }
    if (directive->type == DATA)
    {
        info("Validating type %s\n", directive->type_str);
        validate_data_params(assembler, directive);
        info("Validated %s\n", statement->line);
    }
    if (directive->type == STRING)
    {
        info("Validating type %s\n", directive->type_str);
        validate_string_params(assembler, directive);
        info("Validated %s\n", statement->line);
    }
    if (directive->type == ENTRY)
    {
        info("Validating type %s\n", directive->type_str);
        validate_entry_extern_params(assembler, directive);
        info("Validated %s\n", statement->line);
    }
    if (directive->type == EXTERNAL)
    {
        info("Validating type %s\n", directive->type_str);
        validate_entry_extern_params(assembler, directive);
        info("Validated %s\n", statement->line);
    }
    free(directive);
}

Directive *new_directive(Statement *statement)
{
    Directive *directive = (Directive *)malloc(sizeof(Directive));
    directive->symbol = statement->symbol;
    char *directive_name = get_directive_name(statement);
    directive->type_str = directive_name;
    directive->type = string_to_directive(directive_name);
    directive->params = get_directive_params(statement);
    debug("Params are : %s", directive->params);
    return directive;
}
char *get_directive_name(Statement *statement)
{
    char *line = get_command_line(statement);
    char *first_word = strtok(line, " "); // Extract the first word

    if (first_word == NULL)
    {
        error("write log here");
    }

    return first_word;
}

char *get_directive_params(Statement *statement)
{
    char *line = get_command_line(statement);
    info("Command line is : %s", line);
    // char *first_word = strtok(line, " "); // Extract the first word
    return removeFirstNWords(line, 1);
}

DirectiveMap directive_mapping[] = {
    {".data", DATA},
    {".string", STRING},
    {".entry", ENTRY},
    {".extern", EXTERNAL}};

DirectiveType string_to_directive(const char *str)
{
    for (DirectiveType i = 0; i < sizeof(directive_mapping) / sizeof(DirectiveMap); i++)
    {
        if (strcmp(directive_mapping[i].str, str) == 0)
        {
            return directive_mapping[i].type;
        }
    }
    return -1;
}

void validate_data_params(Assembler *assembler, Directive *directive)
{
    // get the cmd line without .data entry
    char *entry = strdup(directive->params);
    size_t length = strlen(entry);
    int isDigitPresent = 0;
    int isCommaExpected = 0;

    for (size_t i = 0; i < length; i++)
    {
        char currentChar = entry[i];

        // Skip whitespace characters
        if (isspace(currentChar))
        {
            continue;
        }

        // Check for digits
        if (isdigit(currentChar))
        {
            isDigitPresent = 1;
            isCommaExpected = 1;
            continue;
        }

        // Check for commas
        if (currentChar == ',')
        {
            // Check if a comma is expected
            if (!isCommaExpected)
            {
                printf("Invalid data entry: Unexpected comma at position %zu\n", i);
                return;
            }

            // Check if the comma is the last character
            if (i == length - 1)
            {
                printf("Invalid data entry: Comma found at the end of the entry\n");
                return;
            }

            // Check if the character after the comma is a digit
            if (!isdigit(entry[i + 1]))
            {
                printf("Invalid data entry: No digit found after comma at position %zu\n", i);
                return;
            }

            isCommaExpected = 0;
            continue;
        }

        // Invalid character found
        printf("Invalid data entry: Invalid character '%c' at position %zu\n", currentChar, i);
        return;
    }

    // Check if at least one digit is present
    if (!isDigitPresent)
    {
        printf("Invalid data entry: No digits present\n");
    }

    // return isDigitPresent;
    return;
}

// todo :: rewrite method and add errors
int validate_string_params(Assembler *assembler, Directive *directive)
{
    const char *start = directive->params;

    // Check for the opening quotation mark
    if (*start != '\"')
    {
        error("Invalid: Missing opening quotation mark.\n");
        return 0;
    }
    start++;

    // Check for valid characters inside the quotation marks
    while (*start != '\"')
    {
        if (!isalpha(*start))
        {
            error("Invalid: Non-alphabetic character found inside quotation marks.\n");
            return 0;
        }
        start++;
    }

    // Check for trailing characters after the closing quotation mark
    start++;
    while (*start != '\0')
    {
        if (!isspace(*start))
        {
            error("Invalid: Trailing characters found after closing quotation mark.\n");
            return 0;
        }
        start++;
    }

    return 1;
}

int validate_entry_extern_params(Assembler *assembler, Directive *directive)
{
    if (directive->symbol != NULL)
    {
        warn("This assembler is not supporting symbols for .entry or .extern and so this symbol will be ignored");
    }
    char *input = strdup(directive->params);
    if (input == NULL)
    {
        printf("Invalid: Input is NULL.\n");
        return; // Input is NULL
    }

    int hasNonWhitespaceChar = 0; // Flag to track non-whitespace character

    // Check if each character is an alphabetic character or whitespace
    for (const char *ch = input; *ch != '\0'; ch++)
    {
        if (!isspace(*ch))
        {
            hasNonWhitespaceChar = 1;
            if (!isalpha(*ch))
            {
                printf("Invalid: Non-alphabetic non-whitespace character found.\n");
                return; // Non-alphabetic non-whitespace character found
            }
        }
    }

    if (!hasNonWhitespaceChar)
    {
        printf("Invalid: Input does not contain any non-whitespace character.\n");
        return; // Input contains no non-whitespace character
    }

    return; // Input is valid
}
