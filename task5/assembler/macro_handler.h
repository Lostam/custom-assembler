#ifndef MACRO_HANDLER_H
#define MACRO_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_handler.h"
#include "assembler.h"
#include "macros.h"

typedef struct macro_s Macro;

struct macro_s{
    char** content;
    char name[MAX_LINE_LENGTH];
    size_t size;
    size_t capacity;
    Macro *next;
};

typedef struct {
    Macro *tail;  
    Macro *head;
} MacroTable;

void parse_macros(Assembler *assembler);

FileContent* get_parsed_file(Assembler *assembler);

Macro* new_macro(char *header_line);

MacroTable* new_macro_table();

void add_macro_to_table(MacroTable *table, Macro *macro);

Macro* find_macro(MacroTable *table, const char *string);

int is_mcro_usage(const char *line);

int is_macro(const char line[MAX_LINE_LENGTH]);

int is_end_macro(const char line[MAX_LINE_LENGTH]);

void add_macro_to_content(FileContent *file_content, Macro *macro);

void add_line_to_macro(Macro *macro, char *line);

void free_macro_table(MacroTable *table);

void free_macro(Macro *macro);


#endif /* MACRO_HANDLER_H */


