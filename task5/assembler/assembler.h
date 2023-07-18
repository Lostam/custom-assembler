#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>

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

/**
 * @brief Represents a mapping between a label and its corresponding line number.
 */
typedef struct label_line_num_map {
    char* label;    /**< The label string. */
    int line;       /**< The line number associated with the label. */
} LabelToLineMap;

/**
 * @brief Represents an assembler object.
 */
typedef struct {
    int IC;                             /**< The instruction counter. */
    int DC;                             /**< The data counter. */
    LabelToLineMap label_mapper;        /**< The mapping of labels to line numbers. */
    IterationStep iteration_step;       /**< The current iteration step. */
    char* error;       /**< The error messages, null if there are no errors. */
    FILE* file;       
} Assembler;

Assembler* create_assembler(FILE *);
void destroy_assembler(Assembler *);
void syntax_validation(Assembler *);
void add_error(Assembler *,const char *, ...);

#endif /* ASSEMBLER_H */
