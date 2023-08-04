#include "assembler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_builder.h"
#include "logger.h"
#include "macros.h"
#include "statement_handler.h"
#include "symbol_handler.h"
#include "executor.h"

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
    assembler->symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
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

/**
 * Todo :: add doc
 * create close_assembler method
 */
// todo :: convert to linked list
void add_error(Assembler *assembler, const char *unformatted_error, ...) {
    // log the line number
    // log the line content
    assembler->has_error = 1;
    va_list args;
    va_start(args, unformatted_error);

    char error[300];  // todo :: check size
    vsnprintf(error, sizeof(error), unformatted_error, args);
    va_end(args);
    strcat(error, "\n");  // todo :: add line number
    strcat(assembler->error, error);
}

Assembler *new_assembler(char *filename) {
    Assembler *assembler = (Assembler *)malloc(sizeof(Assembler));
    assembler->has_error = 0;
    assembler->file_basename = filename;
    // use constant
    assembler->error = malloc(200); // todo :: use linked list
    assembler->words_l_list = new_linked_list();
    return assembler;
}
