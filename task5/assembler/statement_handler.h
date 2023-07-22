#ifndef STATEMENT_UTILS_H
#define STATEMENT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"


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
    const char* line;
    StatementType type;
    StatementGoal goal;
    const char *symbol; // is it needed
} Statement;

void set_statement_values(Statement *,const char *);
Statement *new_statement(const char *);
char *get_command_line(Statement *);
void validate_statement(Assembler *);
void set_statement_goal(Statement *)

#endif /* STATEMENT_UTILS_H */


