#ifndef STATEMENT_UTILS_H
#define STATEMENT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    STATEMENT_TYPE_EMPTY,
    STATEMENT_TYPE_DIRECTIVE,
    STATEMENT_TYPE_INSTRUCTION,
    STATEMENT_TYPE_COMMENT,
    STATEMENT_UNKNOWN
} StatementType;

// bad name, StatementType is already taken
typedef enum {
    STATEMENT_GOAL_NONE,
    STATEMENT_GOAL_DATA,
    STATEMENT_GOAL_INSTRUCTION,
} StatementGoal;

typedef struct {
    char* line; /**< the statemenet full line, including symbol */
    StatementType type;
    StatementGoal goal; /**< MUST CALL set_statement_goal to use */
    char *symbol; /**< The symbol at the start of the statement, if exists */
} Statement;

#include "assembler.h"

Statement *new_statement(const char *);
/**
    @brief Sets all statemnt values besides goal
*/
void set_statement_values(Statement *,const char *);
/**
    @brief Returns the statement without it symbol(i.e the "command")
*/
char *get_command_line(Statement *);
/**
    @brief Sets the goal of the statement, either data or instruction - used for word counting
*/
void set_statement_goal(Statement *);
void validate_statement(Assembler *);
void free_statement(Statement *);

#endif /** STATEMENT_UTILS_H */


