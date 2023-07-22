#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statement_handler.h"
#include "assembler.h"
#include "symbol_handler.h"
#include "logger.h"

int MAX_LINE_LENGTH = 80;

/**
 * This method is doing a syntax validation of the file
*/
void syntax_validation(Assembler *assembler) {
    FILE* file = fopen("example.txt", "r");
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 100;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        assembler->current_line_number++;
        validate_statement(assembler);
    }
    error("error is %s\n", assembler->error);
}

/**
 * This step is resposible for collecting the data before building the files, 
 * it will fill the symbol table with all the declared symbols and will also count the 
 * instructions and data while not building them yet.
*/
void data_collection(Assembler *assembler) {
    assembler->symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 100;
    while (fgets(line, MAX_LINE_LENGTH, assembler->current_file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        collect_data(assembler);
        assembler->current_line_number++;
    }
}

void build_files(Assembler *assembler) {
    char line[MAX_LINE_LENGTH];
    assembler->current_line_number = 100;
    while (fgets(line, MAX_LINE_LENGTH, assembler->current_file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        validate_statement(assembler);
        assembler->current_line_number++;
    }
}


/**
 * Todo :: add doc
 * create close_assembler method
*/
void add_error(Assembler *assembler, const char *unformatted_error, ...) {
    // log the line number
    // log the line content
    va_list args;
    va_start(args, unformatted_error);

    char error[100]; // todo :: check size
    vsnprintf(error, sizeof(error), unformatted_error, args);

    va_end(args);
    strcat(assembler->error , error);
}