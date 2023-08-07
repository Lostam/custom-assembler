#ifndef MACRO_HANDLER_H
#define MACRO_HANDLER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "file_handler.h"
#include "macros.h"

typedef struct macro_s Macro;

struct macro_s {
    char **content;
    char name[MAX_LINE_LENGTH];
    size_t size;
    size_t capacity;
    Macro *next;
};

typedef struct {
    Macro *tail;
    Macro *head;
} MacroTable;

/**
    @brief File's orchestrator
*/
void parse_macros(Assembler *assembler);

/**
    @brief Builds an .am FileContent object from the .as file
*/
FileContent *get_parsed_file(Assembler *assembler);

/**
    @brief Adds macro to the macro table
*/
void add_macro_to_table(MacroTable *table, Macro *macro);

/**
    @brief Finds a macro from the macro table by string
*/
Macro *find_macro(MacroTable *table, const char *string);

/**
    @brief Returns wether a line is using mcro
*/
int is_mcro_usage(const char *line);

/**
    @brief Returns wether a line is defining a mcro
*/
int is_macro(const char line[MAX_LINE_LENGTH]);

/**
    @brief Returns wether a line is an endmcro
*/
int is_end_macro(const char line[MAX_LINE_LENGTH]);

/**
    @brief Adds a macro to the file content instead of it usage (e.g replaces "m1" with the content of "mcro m1")
*/
void add_macro_to_content(FileContent *file_content, Macro *macro);
/**
    @brief Adds a line to a macro to later replace a macro usage
*/
void add_line_to_macro(Macro *macro, const char *line);

MacroTable *new_macro_table();
Macro *new_macro(const char *header_line);
void free_macro_table(MacroTable *table);
void free_macro(Macro *macro);
/**
 * @private, for cleaner code
*/
void parse_line(Assembler* assembler, MacroTable* table, FileContent* parsed_file, char* line, int* in_macro);

#endif /** MACRO_HANDLER_H */
