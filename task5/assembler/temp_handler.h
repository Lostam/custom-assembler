#ifndef TEMP_HANDLER_H
#define TEMP_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "statement_handler.h"


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

void collect_data(Assembler *);

#endif /* TEMP_HANDLER_H */


