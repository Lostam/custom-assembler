#include "executor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "macros.h"
#include "statement_handler.h"
#include "symbol_handler.h"
#include "macro_handler.h"
#include "file_builder.h"
#include "utils.h"
#include "word_handler.h"


void run(char *files[], size_t size) {
    for (int i = 1; i < size; i++) {
        info("--------------------------------------------------------------");
        info("--------------------------------------------------------------");
        info("---------------Starting Working On File %s---------------", files[i]);
        Assembler *assembler = new_assembler(files[i]);
        info("---------------Starting Macro Parsing---------------");
        parse_macros(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        set_open_file(assembler);
        info("---------------Starting Syntax Validation---------------");
        syntax_validation(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting First Data Collection---------------");
        data_collection(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting Second Data Collection---------------");
        build_data_for_files(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting File Building---------------");
        generate_output_files(assembler);
        free_assembler(assembler);
        printf("\n\n\n");
        return;
    }
}

int clear_if_error(Assembler *assembler) {
    if (assembler->has_error) {
        error("Errors found for provided file %s are :", assembler->file_basename);
        Node *current = assembler->error_l_list->head;
        int i = 0;
        while (current != NULL) {
            error("%d : %s", ++i, current->content);
            current = current->next;
        }
        printf("\n\n\n");
        free_assembler(assembler);
        return 1;
    }
    return 0;
}

/**
 * Validation each line, will build the statements, directives and instructions and validate them
 */
void syntax_validation(Assembler *assembler) {
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 0;
    while (fgets(line, MAX_LINE_LENGTH, assembler->as_file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        assembler->current_line_number++;
        validate_statement(assembler);
    }
}

/**
 * This step is resposible for collecting the data before building the files,
 * it will fill the symbol table with all the declared symbols and will also count the
 * instructions and data words while not building them yet.
 */
void data_collection(Assembler *assembler) {
    assembler->IC = 0;
    assembler->DC = 0;
    fseek(assembler->as_file, 0, SEEK_SET);
    assembler->symbol_table = new_symbol_table();
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 0;
    while (fgets(line, MAX_LINE_LENGTH, assembler->as_file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        first_data_collection(assembler);
        assembler->current_line_number++;
    }
    add_counter_to_data(assembler);
}

/**
 * Building the words from instructions and directives and will mark entries/externals
 */
void build_data_for_files(Assembler *assembler) {
    assembler->IC = 0;
    fseek(assembler->as_file, 0, SEEK_SET);
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 0;
    while (fgets(line, MAX_LINE_LENGTH, assembler->as_file) != NULL) {
        size_t length = strlen(line);
        assembler->current_line_number++;
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        second_data_collection(assembler);
    }
}


void first_data_collection(Assembler *assembler) {
    info("Starting first run for line %s, line: %d", assembler->current_line, assembler->current_line_number);
    Statement *statement = new_statement(assembler->current_line);
    if (statement->type == STATEMENT_TYPE_COMMENT || statement->type == STATEMENT_TYPE_EMPTY) {
        return;
    }
    set_statement_goal(statement);
    collect_symbols_from_statement(assembler, statement);
    int num_of_words = count_required_words(statement);
    if (statement->goal == STATEMENT_GOAL_DATA) {
        assembler->DC += num_of_words;
    }
    if (statement->goal == STATEMENT_GOAL_INSTRUCTION) {
        assembler->IC += num_of_words;
    }
    if (num_of_words >= 1024) {
        add_error(assembler, "Number of words suppressed the maximum allowed of 1024 in statement %s", assembler->current_line);
        return;
    }
}

void second_data_collection(Assembler *assembler) {
    info("Starting second run for line %s, line: %d", assembler->current_line, assembler->current_line_number);
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
        if (directive->type == DIRECTIVE_TYPE_ENTRY) {
            debug("Marking entry for line %s", assembler->current_line);
            char *copy = strdup(directive->params);
            char *token = strtok(copy, ",");
            while (token != NULL) {
                Symbol *symbol = get_symbol_by_name(assembler->symbol_table, trim_spaces(token));
                if (symbol != NULL) {
                    info("Marking entry %s", symbol->name);
                    symbol->is_entry = 1;
                } else {
                    warn("Symbol %s was declared as an entry symbol but wasn't used in the program", token);
                }
                token = strtok(NULL, ",");
            }
            free(copy);
        }
        free_directive(directive);
    }
}

void mark_external(Assembler *assembler, Statement *statement) {
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *instruction = new_instruction(statement);
        Operand *destination = instruction->destination;
        Operand *source = instruction->source;
        int destination_counter_base = source->type == UNDEFINED_MODE ? 1 : 2;
        mark_external_if_needed(assembler, destination, destination_counter_base);
        mark_external_if_needed(assembler, source, 1);
        free_instruction(instruction);
    }
}

void mark_external_if_needed(Assembler *assembler, Operand *operand, int counter_base) {
    if (operand->type == DIRECT_MODE) {
        debug("Getting symbol [%s] of type [%d]", operand->value, operand->type);
        Symbol *symbol = get_symbol_by_name(assembler->symbol_table, operand->value);
        if (symbol == NULL) {
            add_error(assembler, "Symbol [%s] was not declared in the .as file correctly", operand->value);
            return;
        }
        if (symbol->type == SYMBOL_TYPE_EXTERNAL) {
            char *message = (char *)malloc(MAX_LINE_LENGTH);
            sprintf(message, "%s %d", symbol->name, assembler->IC + counter_base + BASE_MEMORY_COUNTER);
            info("Saving external symbol file line [%s]", message);
            add_string_to_list(symbol->used_at, message);
        }
    }
}