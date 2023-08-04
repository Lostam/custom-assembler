#include "directive_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "statement_handler.h"
#include "string_utils.h"
#include "symbol_handler.h"

#define VALID_DIRECTIVES ".data, .string, .entry, .extern"

// assume the structure of the line is : "OPTIONAL_LABEL: .<SOME_STRING>..."
void validate_directive(Assembler *assembler, Statement *statement) {
    Directive *directive = new_directive(statement);
    if (directive->type == -1) {
        add_error(assembler, "Found invalid directive .%s, valid directives are %s", directive->type_str, VALID_DIRECTIVES);
        return;
    }
    if (directive->type == DIRECTIVE_TYPE_DATA) {
        validate_data_params(assembler, directive);
        info("Validated %s", statement->line);
    }
    if (directive->type == DIRECTIVE_TYPE_STRING) {
        validate_string_params(assembler, directive);
        info("Validated %s", statement->line);
    }
    // todo :: join 2 ifs
    if (directive->type == DIRECTIVE_TYPE_ENTRY || directive->type == DIRECTIVE_TYPE_EXTERNAL) {
        validate_entry_extern_params(assembler, directive);
        info("Validated %s\n", statement->line);
    }
    free_directive(directive);
}

Directive *new_directive(Statement *statement) {
    Directive *directive = (Directive *)malloc(sizeof(Directive));
    directive->symbol = statement->symbol;
    char *directive_name = get_directive_name(statement);
    directive->type_str = directive_name;
    directive->type = string_to_directive(directive_name);
    directive->params = get_directive_params(statement);
    return directive;
}
char *get_directive_name(Statement *statement) {
    char *line = get_command_line(statement);
    char *first_word = strtok(line, " ");  // Extract the first word

    if (first_word == NULL) {
        error("write log here");
    }

    return first_word;
}

char *get_directive_params(Statement *statement) {
    char *line = get_command_line(statement);
    // char *first_word = strtok(line, " "); // Extract the first word
    return trim_spaces(remove_first_n_words(line, 1));
}

DirectiveMap directive_mapping[] = {
    {".data", DIRECTIVE_TYPE_DATA},
    {".string", DIRECTIVE_TYPE_STRING},
    {".entry", DIRECTIVE_TYPE_ENTRY},
    {".extern", DIRECTIVE_TYPE_EXTERNAL}};

DirectiveType string_to_directive(const char *str) {
    for (DirectiveType i = 0; i < sizeof(directive_mapping) / sizeof(DirectiveMap); i++) {
        if (strcmp(directive_mapping[i].str, str) == 0) {
            return directive_mapping[i].type;
        }
    }
    return -1;
}

// todo :: make sure -,+ are also valid,
// todo :: make sure the is smaller then 65535(1111111111111111)
void validate_data_params(Assembler *assembler, Directive *directive) {
    char *entry = strdup(directive->params);
    char* token;
    char delimiters[] = ",";
    int expecting_digit = 1; 

    token = strtok(entry, delimiters);
    while (token != NULL) {
        char* endptr;
        int is_whole = is_whole_number(token);

        if (!is_whole) {
            add_error(assembler, "Invalid entry: '%s' is not a valid number.\n", token);
            break;
        }

        expecting_digit = 0;

        token = strtok(NULL, delimiters);
        
        if (token != NULL && strcmp(token, ",")) {
            expecting_digit = 1;
            token = strtok(NULL, delimiters);
        } else if (token != NULL) {
            add_error(assembler, "Invalid entry: Expecting a comma after the number '%s'.\n", token);
            break;
        }
    }

    if (expecting_digit) {
        add_error(assembler, "Invalid entry: Missing number after the last comma.\n");
    }
    free(entry);
}

// todo :: make sure there are no 3 "
int validate_string_params(Assembler *assembler, Directive *directive) {
    const char *start = directive->params;
    if (*start != '\"') {
        add_error(assembler, "Invalid string directive, missing opening qoutation mark.\n");
        return 0;
    }
    start++;

    while (*start != '\"') {
        if (!isalpha(*start)) {
            add_error(assembler, "Only alphabetic values are allowed inside string directive qoutation marks, found %s", start);
            return 0;
        }
        start++;
    }
    // todo :: make sure to check for closing "
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
    // todo :: check multiple labels
    char *input = strdup(directive->params);
    if (is_empty(input)) {
        add_error(assembler, ".extern and .entry directives must include at least label");
        free(input);
        return;
    }
    if (!is_valid_label(input)) {
        add_error(assembler, "Invalid: label name [%s]\n", assembler->current_line);
        free(input);
        return;
    }
    free(input);
    return;
}

void free_directive(Directive *directive) {
    free(directive->params);
    free(directive->symbol);
    free(directive->type_str);
    free(directive);
}