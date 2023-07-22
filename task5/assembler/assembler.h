#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/**
 * @brief Represents the possible iteration steps.
 */
typedef enum itr_step {
    PRE_MACRO,      /**< Pre-macro iteration step. */
    SYNTAX_VALIDATION,      
    SYMBOL_LOADING,     
    TABLE_CREATING,    
    FILES_CREATING,     
} IterationStep;

// /**
//  * @brief Represents a mapping between a label and its corresponding Symbol instance.
//  */
// // change name to SymbolEntry
// typedef struct {
//     char* str;    /**< The label string. */
//     Symbol symbol;       /**< The line number associated with the label. */
// } SymbolKV;

typedef struct {
    size_t number;
    char *content;       
} Line;


/**
 * @brief Represents an assembler object.
 */
typedef struct {
    int IC;                             /**< The instruction counter. */
    int DC;                             /**< The data counter. */
    SymbolTable *symbol_table;        /**< The mapping of labels to line numbers. */
    IterationStep iteration_step;       /**< The current iteration step. */
    char* error;       /**< The error messages, null if there are no errors. */
    FILE* current_file;       
    char *current_line; // limit to 100
    char *current_line_number;
} Assembler;

Assembler* create_assembler(FILE *);
void close_assembler(Assembler *);
void syntax_validation(Assembler *);
void add_error(Assembler *,const char *, ...);
void data_collection(Assembler *);
void build_files(Assembler *);

#endif /* ASSEMBLER_H */
