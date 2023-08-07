#include "assembler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "macros.h"

/**
 * This method will add the error to a linked list, the linked list is saved on the assembler object itself
 */
void add_error(Assembler *assembler, const char *unformatted_error, ...) {
    assembler->has_error = 1;

    va_list args;
    va_start(args, unformatted_error);
    char error[300];  // Limiting error size to 300 chars
    vsnprintf(error, sizeof(error), unformatted_error, args);
    va_end(args);

    char *message = (char *)malloc(MAX_LINE_LENGTH + 100 + sizeof(error));
    sprintf(message, "Line Number : [%d], line content : [%s] \n %s \n", assembler->current_line_number, assembler->current_line, error);
    add_string_to_list(assembler->error_l_list, message);

    free(message);
}

Assembler *new_assembler(char *filename) {
    Assembler *assembler = (Assembler *)malloc(sizeof(Assembler));
    if (assembler == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    assembler->has_error = 0;
    assembler->file_basename = strdup(filename);
    assembler->error_l_list = new_linked_list();
    assembler->words_l_list = new_linked_list();
    assembler->as_file = NULL;
    return assembler;
}

void free_assembler(Assembler *assembler) {
    if (assembler->as_file != NULL) {
        fclose(assembler->as_file);
    }
    if (assembler->symbol_table != NULL) {
        free_symbol_table(assembler->symbol_table);
    }
    if (assembler->file_basename != NULL) {
        free(assembler->file_basename);
    }
    free_linked_list(assembler->error_l_list);
    free_linked_list(assembler->words_l_list);
    free(assembler);
}
