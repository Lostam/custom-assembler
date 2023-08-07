#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct assembler_s Assembler;

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "symbol_handler.h"
#include "word_handler.h"

/**
 * @brief Represents an assembler object.
 */
struct assembler_s {
    int IC;                         /**< The instruction counter. */
    int DC;                         /**< The data counter. */

    // maybe I should have used hashmap?
    SymbolTable *symbol_table;      /**< The mapping of labels to line numbers. */
    
    StringLinkedList *words_l_list; /**< Linked list of built words. */
    StringLinkedList *error_l_list; /**< The error messages, empty if there are no errors. */
    int has_error;                  /**< marks if an error exists. */
    FILE *as_file;                  /**< the .as file, it is used alot so it's better to save it. */
    char *file_basename;            /**< the file base name as passed by the user. */
    char *current_line;             /**< content of the current working line. */
    int current_line_number;        /**< number of the current working line - if in .am file it will the line number of the .am file! */
};

void close_assembler(Assembler *);
void add_error(Assembler *, const char *, ...);
Assembler *new_assembler(char *);
void free_assembler(Assembler *);

#endif /* ASSEMBLER_H */
