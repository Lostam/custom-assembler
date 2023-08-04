#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct assembler_s Assembler;

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "word_handler.h"
#include "symbol_handler.h"

typedef struct {
    size_t number;
    char *content;
} Line;

/**
 * @brief Represents an assembler object.
 */
struct assembler_s {
    int IC;                       /**< The instruction counter. */
    int DC;                       /**< The data counter. */
    SymbolTable *symbol_table;    /**< The mapping of labels to line numbers. */
    StringLinkedList *words_l_list;    /**< The mapping of labels to line numbers. */
    char *error;                  /**< The error messages, null if there are no errors. */
    int has_error;                  /**< The error messages, null if there are no errors. */
    FILE *as_file;
    char *file_basename;
    char *current_line;  // limit to 100 // make a struct
    int current_line_number;
};

Assembler *create_assembler(FILE *);
void close_assembler(Assembler *);
void syntax_validation(Assembler *);
void add_error(Assembler *, const char *, ...);
void data_collection(Assembler *);
void build_data_for_files(Assembler *);
Assembler *new_assembler(char *);

// todo:: check words are not larger then 1024
#endif /* ASSEMBLER_H */
