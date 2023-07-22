#ifndef SYMBOL_HANDLER_H
#define SYMBOL_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "statement_handler.h"


typedef enum {
    SYMBOL_TYPE_RELOCATABLE, // a symbol declared locally or using .entry
    SYMBOL_TYPE_EXTERNAL, // a symbol declared using .extern
} SymbolType;

typedef enum {
    SYMBOL_SIGN_DATA, // a symbol declared before instruction
    SYMBOL_SIGN_CODE, // a symbol declared before data directive
} SymbolSign;

typedef struct {
    const char* line_number; // line where the symbol was declared
    SymbolType type; // either external or internal
    SymbolSign sign; // either data or code tyoe
    int is_entry; // was declared in an entry directive?
    const char *name; // the symbol string
} Symbol;

typedef struct {
    Symbol **symbols;    /**< The label string. */
    size_t size;       /**< The line number associated with the label. */
    size_t capacity;       /**< The line number associated with the label. */
} SymbolTable;

void collect_symbol(Assembler *, Statement *statement);
void add_to_table(SymbolTable *, Symbol *);
int is_symbol_in_map(SymbolTable *, char *);

#endif /* SYMBOL_HANDLER_H */


