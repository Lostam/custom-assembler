#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statement_handler.h"
#include "word_handler.h"
#include "assembler.h"
#include "symbol_handler.h"
#include "logger.h"

void collect_data(Assembler *assembler) {
    debug("Starting collecting data for line %d", assembler->current_line_number);
    Statement *statement = new_statement(assembler->current_line);
    set_statement_goal(statement);
    collect_symbol(assembler, statement);
    int num_of_words = count_required_words(statement);
    if (statement->goal == STATEMENT_GOAL_DATA) {
        assembler->DC+=num_of_words;
    }
    if (statement->goal == STATEMENT_GOAL_INSTRUCTION) {
        assembler->IC+=num_of_words;
    }
}
