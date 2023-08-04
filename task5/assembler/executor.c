#include "executor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "statement_handler.h"
#include "symbol_handler.h"
#include "macros.h"
#include "utils.h"
#include "word_handler.h"

void first_data_collection(Assembler *assembler) {
    Statement *statement = new_statement(assembler->current_line);
    set_statement_goal(statement);
    collect_symbol(assembler, statement);
    int num_of_words = count_required_words(statement);
    if (statement->goal == STATEMENT_GOAL_DATA) {
        assembler->DC += num_of_words;
    }
    if (statement->goal == STATEMENT_GOAL_INSTRUCTION) {
        assembler->IC += num_of_words;
    }
}

// todo change name
void second_data_collection(Assembler *assembler) {
    debug("Starting second run for line %s, line: %d", assembler->current_line, assembler->current_line_number);
    Statement *statement = new_statement(assembler->current_line);
    mark_entry(assembler, statement);
    mark_external(assembler, statement);
    add_words_to_assembler(assembler, statement);
    // recounting IC for external symbols collection
    set_statement_goal(statement);
    if (statement->goal == STATEMENT_GOAL_INSTRUCTION) {
        int num_of_words = count_required_words(statement);
        assembler->IC += num_of_words;
    }
}

void mark_entry(Assembler *assembler, Statement *statement) {
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        // todo :: support multiple
        if (directive->type == DIRECTIVE_TYPE_ENTRY) {
            debug("Marking entry for line %s", assembler->current_line);
            char *copy = strdup(directive->params);
            Symbol *symbol = get_symbol_by_name(assembler->symbol_table, trim_spaces(copy));
            if (symbol != NULL) {
                symbol->is_entry = 1;
            }
            free(copy);
            // do something incase not found?
        }
        free_directive(directive);
    }
}

void mark_external(Assembler *assembler, Statement *statement) {
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *instruction = new_instruction(statement);
        Operand *destination = instruction->destination;
        Operand *source = instruction->source;
        int destination_counter_base = source->type == Undefined ? 1 : 2;
        mark_external_if_needed(assembler, destination , destination_counter_base);
        mark_external_if_needed(assembler, source, 1);
        free_instruction(instruction);
    }
}

void mark_external_if_needed(Assembler *assembler, Operand *operand, int counter_base) {
    if (operand->type == Direct) {
        Symbol *symbol = get_symbol_by_name(assembler->symbol_table, operand->value);
        if (symbol->type == SYMBOL_TYPE_EXTERNAL) {
            char *message = (char *)malloc(MAX_LINE_LENGTH);
            sprintf(message, "%s %d", symbol->name, assembler->IC + counter_base + 100);
            debug("Saving external symbol file line [%s]", message);
            add_string_to_list(symbol->used_at, message);
        }
    }
}