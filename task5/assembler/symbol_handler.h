#ifndef STATEMENT_UTILS_H
#define STATEMENT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"


typedef enum {
    Reloca,
    SYMBOL_TYPE_RELOCATABLE,
    SYMBOL_TYPE_EXTERNAL
} SymbolType;

typedef struct {
    const char* value;
    SymbolType type;
    const char *name; 
} Symbol;

void collect_symbol(Assembler *);
void add_to_table(SymbolTable *, SymbolKV *);
int is_symbol_in_map(SymbolTable *, char *);

#endif /* STATEMENT_UTILS_H */


