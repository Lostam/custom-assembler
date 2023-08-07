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



Statement *new_statement(const char *line) {
    Statement *statement = (Statement *)malloc(sizeof(Statement));
    if (statement == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    statement->line = strdup(line);
    statement->symbol = NULL;
    set_statement_values(statement, line);
    return statement;
}

void validate_statement(Assembler *assembler) {
    Statement *statement = new_statement(assembler->current_line);
    info("statement created [%s] for line number %d", assembler->current_line, assembler->current_line_number);
    if (statement->type == STATEMENT_UNKNOWN) {
        add_error(assembler, "Failed parsing statement : %s", assembler->current_line);
        free_statement(statement);
        return;
    }
    if (statement->symbol != NULL) {
        validate_label(assembler, statement->symbol);
    }
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        debug("Will try and validate directive %s", statement->line);
        validate_directive(assembler, statement);
    }

    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        debug("Will try and validate instruction %s", statement->line);
        validate_instruction(assembler, statement);
    }
    free_statement(statement);
}

void set_statement_values(Statement *statement, const char *original_line) {
    char *line = trim_spaces(strdup(original_line));
    int empty = 1;
    statement->line = strdup(original_line);
    statement->symbol = NULL;
    if (is_empty(line)) {
        statement->type = STATEMENT_TYPE_EMPTY;
        free(line);
        return;
    }
    if (line[0] == ';') {
        statement->type = STATEMENT_TYPE_COMMENT;
        free(line);
        return;
    }
    char *word = strtok(line, " ");
    int len = strlen(word);
    int is_symbol = word[len - 1] == ':';
    if (is_symbol) {
        word[len - 1] = '\0';
        statement->symbol = strdup(word);
        word = strtok(NULL, " ");
    }

    if (word == NULL) {
        statement->type = STATEMENT_UNKNOWN;
    }
    if (word[0] == '.') {
        statement->type = STATEMENT_TYPE_DIRECTIVE;
    }
    else if (is_valid_operation_name(word)) {
        statement->type = STATEMENT_TYPE_INSTRUCTION;
    } else {
        statement->type = STATEMENT_UNKNOWN;
    }
    free(line);
    return;
}

char *get_command_line(Statement *statement) {
    char *cmd_line = strdup(statement->line);
    if (statement->symbol == NULL) {
        return cmd_line;
    }
    char *occurrence = strstr(cmd_line, statement->symbol);
    if (occurrence != NULL) {
        size_t symbol_len = strlen(statement->symbol) + 1;  // adding 1 for the missing :
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
            statement->goal = STATEMENT_GOAL_DATA;
        }
        free_directive(directive);
    }
}

void free_statement(Statement *statement) {
    if (statement->symbol != NULL) {
        free(statement->symbol);
    }
    free(statement->line);
    free(statement);
}