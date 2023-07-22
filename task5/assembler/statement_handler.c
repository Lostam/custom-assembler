#include "statement_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "directive_handler.h"
#include "instruction_handler.h"
#include "logger.h"
#include "operation_handler.h"

// create a mapping the string representation of the operation to the operation enum, also write a method which takes as parameter a string and returns the operation enum

Statement *new_statement(const char *line) {
    Statement *statement = (Statement *)malloc(sizeof(Statement));
    statement->line = line;
    // check for null
    set_statement_values(statement, line);
    return statement;
    // remember to free statement
}

void validate_statement(Assembler *assembler) {
    Statement *statement = new_statement(assembler->current_line);
    info("statement created [%s]", assembler->current_line);
    if (statement->type == STATEMENT_UNKNOWN) {
        // todo :: improve error log
        warn("Invalid statement found [%s]", statement->line);
        add_error(assembler, "Failed parsing line _addline_\n");
        info("Freeing statememnt [%s]", statement->line);
        free(statement);
        return;
    }
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        validate_directive(assembler, statement);
    }

    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        validate_instruction(assembler, statement);
    }
    free(statement);
}

void set_statement_values(Statement *statement, const char *original_line) {
    char *line = strdup(original_line);
    int empty = 1;
    statement->line = original_line;
    statement->symbol = "";
    for (size_t i = 0; i < strlen(line); i++) {
        if (!isspace(line[i])) {
            empty = 0;
            break;
        }
    }
    if (empty) {
        statement->type = STATEMENT_TYPE_EMPTY;
        return;
    }
    if (line[0] == ';') {
        statement->type = STATEMENT_TYPE_COMMENT;
        return;
    }
    char *word = strtok(line, " ");
    int len = strlen(word);
    int is_symbol = word[len - 1] == ':';
    if (is_symbol) {
        debug("Found symbol %s in line %s", word, original_line);
        // todo:: check if is valid symbol
        // check for reserved words
        statement->symbol = word;
        word = strtok(NULL, " ");
    }

    // check for null

    if (word[0] == '.') {
        statement->type = STATEMENT_TYPE_DIRECTIVE;
        return;
    }
    if (is_valid_operation_name(word)) {
        // todo ? add error
        statement->type = STATEMENT_TYPE_INSTRUCTION;
        return;
    }
    statement->type = STATEMENT_UNKNOWN;
    return;
}

char *get_command_line(Statement *statement) {
    char *cmd_line = strdup(statement->line);  // Create a copy of the line
    char *occurrence = strstr(cmd_line, statement->symbol);
    if (occurrence != NULL) {
        size_t symbol_len = strlen(statement->symbol);
        memmove(occurrence, occurrence + symbol_len, strlen(occurrence + symbol_len) + 1);
    }
    return cmd_line;
}

void set_statement_goal(Statement *statement) {
    statement->goal = STATEMENT_GOAL_NONE;
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        statement->goal = STATEMENT_GOAL_INSTRUCTION;
    }
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        if (directive->type == DIRECTIVE_TYPE_DATA || directive->type == DIRECTIVE_TYPE_STRING) {
            statement->goal = STATEMENT_GOAL_INSTRUCTION;
        } 
        free_directive(directive);
    }
}


// add method :: mark_symbol_as_entry