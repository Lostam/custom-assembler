#ifndef SYMBOL_HANDLER_H
#define SYMBOL_HANDLER_H

typedef struct symbol_table_s SymbolTable;
typedef struct symbol_s Symbol;
typedef enum symbol_type_e SymbolType;
typedef enum symbol_sign_e SymbolSign;


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "statement_handler.h"
#include "utils.h"

enum symbol_type_e {
    SYMBOL_TYPE_RELOCATABLE,  // a symbol declared locally or using .entry
    SYMBOL_TYPE_EXTERNAL,     // a symbol declared using .extern
};

enum symbol_sign_e {
    SYMBOL_SIGN_DATA,  // a symbol declared before instruction
    SYMBOL_SIGN_CODE,  // a symbol declared before data directive
};

struct symbol_s {
    int line_number;      // the IC/DC counter of the symbol
    SymbolType type;   // either external or internal
    SymbolSign sign;   // either data or code tyoe
    int is_entry;      // if was declared in an entry directive
    StringLinkedList *used_at;      // Used only for external symbols, a linked list of the symbol's usages
    const char *name;  // the symbol value
};

struct symbol_table_s {
    Symbol **symbols; /**< The label string. */
    size_t size;      /**< The line number associated with the label. */
    size_t capacity;  /**< The line number associated with the label. */
};


void collect_symbol(Assembler *, Statement *);
void add_to_table(Assembler *, Symbol *);
void add_counter_to_data(Assembler *);
int is_symbol_in_map(SymbolTable *, Symbol *);
Symbol *get_symbol_from_statement(Statement *, int, int);
SymbolTable *new_symbol_table(void);
int is_valid_label(const char *);
Symbol *get_symbol_by_name(SymbolTable *,const char *);
void free_symbol_table(SymbolTable *);
void free_symbol(Symbol *);

#endif /* SYMBOL_HANDLER_H */
