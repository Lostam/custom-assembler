#include "directive_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "macros.h"
#include "statement_handler.h"
#include "string_utils.h"
#include "symbol_handler.h"

#define VALID_DIRECTIVES ".data, .string, .entry, .extern"

// assume the structure of the line is : "OPTIONAL_LABEL: .<SOME_STRING>..."
void validate_directive(Assembler *assembler, Statement *statement) {
    Directive *directive = new_directive(statement);

    if (directive->type == DIRECTIVE_TYPE_NOT_FOUND) {
        add_error(assembler, "Found invalid directive [.%s], valid directives are [%s]", directive->type_str, VALID_DIRECTIVES);
        return;
    }
    if (directive->type == DIRECTIVE_TYPE_DATA) {
        validate_data_params(assembler, directive);
        debug("Validated %s", statement->line);
    }
    if (directive->type == DIRECTIVE_TYPE_STRING) {
        validate_string_params(assembler, directive);
        debug("Validated %s", statement->line);
    }
    if (directive->type == DIRECTIVE_TYPE_ENTRY || directive->type == DIRECTIVE_TYPE_EXTERNAL) {
        validate_entry_extern_params(assembler, directive);
        debug("Validated %s", statement->line);
    }
    free_directive(directive);
}

Directive *new_directive(Statement *statement) {
    Directive *directive = (Directive *)malloc(sizeof(Directive));
    if (directive == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    directive->symbol = NULL;
    if (statement->symbol != NULL) {
        directive->symbol = strdup(statement->symbol);
    }
    char *directive_name = get_directive_name(statement);
    directive->type_str = directive_name;
    directive->type = string_to_directive(directive_name);
    directive->params = get_directive_params(statement);
    return directive;
}
char *get_directive_name(Statement *statement) {
    char *line = get_command_line(statement);
    char *first_word = get_first_word(line);
    free(line);
    return first_word;
}

char *get_directive_params(Statement *statement) {
    char *line = get_command_line(statement);
    // char *first_word = strtok(line, " "); // Extract the first word
    char *trimmed = trim_spaces(remove_first_n_words(line, 1));
    free(line);
    return trimmed;
}

DirectiveMap directive_mapping[] = {
    {".data", DIRECTIVE_TYPE_DATA},
    {".string", DIRECTIVE_TYPE_STRING},
    {".entry", DIRECTIVE_TYPE_ENTRY},
    {".extern", DIRECTIVE_TYPE_EXTERNAL}};

DirectiveType string_to_directive(const char *str) {
    if (str == NULL) {
        return DIRECTIVE_TYPE_NOT_FOUND;
    }
    for (DirectiveType i = 0; i < sizeof(directive_mapping) / sizeof(DirectiveMap); i++) {
        if (strcmp(directive_mapping[i].str, str) == 0) {
            return directive_mapping[i].type;
        }
    }
    return DIRECTIVE_TYPE_NOT_FOUND;
}

void validate_data_params(Assembler *assembler, Directive *directive) {
    char *entry = trim_spaces(strdup(directive->params));
    char delimiter[] = ",";
    if (is_empty(entry)) {
        add_error(assembler, "Invalid data entry [%s] : ['%s'] must not be empty", assembler->current_line, entry);
    }
    if (strncmp(entry, delimiter, 1) == 0 || entry[strlen(entry)-1] == ',') {
        add_error(assembler, "Invalid data entry [%s] : '%s' must not start or end with a comma", assembler->current_line, entry);
    }

    char *token = strtok(entry, delimiter);
    while (token != NULL) {
        token = trim_spaces(token);
        if (!is_whole_number(token)) {
            add_error(assembler, "Invalid data entry [%s] : '%s' is not a valid number", assembler->current_line, token);
            break;
        }
        if (atoll(token) > MAXIMUM_ALLOWED_NUMBER) {
            add_error(assembler, "Invalid data entry [%s] : number [%s] is larger then the allowed %d",
                      assembler->current_line, token, MAXIMUM_ALLOWED_NUMBER);
            break;
        }
        token = strtok(NULL, delimiter);
    }
    free(entry);
}

int validate_string_params(Assembler *assembler, Directive *directive) {
    const char *start = directive->params;
    if (*start != '\"') {
        add_error(assembler, "Invalid string directive, missing opening qoutation mark");
        return 0;
    }
    start++;

    while (*start != '\"') {
        if (!is_valid_ascii(*start)) {
            add_error(assembler, "Only ASCII values are allowed inside string directive qoutation marks, found %c", *start);
            return 0;
        }
        start++;
    }

    start++;
    while (*start != '\0') {
        if (!isspace(*start)) {
            add_error(assembler, "Additional character after the closing quotation mark are not allowed, found %s", start);
            return 0;
        }
        start++;
    }

    return 1;
}

void validate_entry_extern_params(Assembler *assembler, Directive *directive) {
    if (!is_empty(directive->symbol)) {
        warn("This assembler is not supporting symbols for .entry or .extern and so this symbol will be ignored");
    }
    // same code as in data, couldn't find a way to abstract it
    char *input = trim_spaces(strdup(directive->params));
    char delimiter[] = ",";
    if (strncmp(input, delimiter, 1) == 0 || input[strlen(input)-1] == ',') {
        add_error(assembler, "Invalid .entery/.extern params [%s] : '%s' must not start or end with a comma", assembler->current_line, input);
    }
    if (is_empty(input)) {
        add_error(assembler, ".extern and .entry directives must include at least one label");
        free(input);
        return;
    }
    char *token;

    token = strtok(input, delimiter);
    while (token != NULL) {
        token = trim_spaces(token);
        validate_label(assembler, token);
        token = strtok(NULL, delimiter);
    }

    free(input);
    return;
}

void free_directive(Directive *directive) {
    if (directive->params != NULL) {
        free(directive->params);
    }
    if (directive->symbol != NULL) {
        free(directive->symbol);
    }
    if (directive->type_str != NULL) {
        free(directive->type_str);
    }
    free(directive);
}