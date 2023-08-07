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
#include "directive_handler.h"
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
    SymbolSign sign;   // either data or code type
    int is_entry;      // if was declared in an entry directive
    StringLinkedList *used_at;      // Used only for external symbols, a linked list of the symbol's usages
    char *name;  // the symbol value
};

struct symbol_table_s {
    Symbol **symbols; /**< The label string. */
    size_t size;      /**< Current size of the label table. */
    size_t capacity;  /**< Maximum current capacity of the table. */
};


/**
    @brief Reads the statement for all the symbols and adds them to the symbols table
*/
void collect_symbols_from_statement(Assembler *, Statement *);
/**
    @brief Adding symbol to symbols table
*/
void add_to_table(Assembler *, Symbol *);
/**
    @brief Adding IC to all the data symbols
*/
void add_counter_to_data(Assembler *);
/**
    @brief Searches the symbol table and returns a symbol or null if not found
*/
Symbol *get_symbol_by_name(SymbolTable *,const char *);
/**
    @brief Reads an external directive and adds it symbols to the table
*/
void add_external_symbols(Assembler *, Directive *);
/**
    @brief Reads a symbol declared in the start of a statement and adds it symbols to the table
*/
void add_regular_symbol(Assembler *, Statement *);
/**
    @brief Returns if symbol is in the map
*/
int is_symbol_in_map(SymbolTable *, Symbol *);
void validate_label(Assembler *, const char *);
Symbol *new_empty_symbol(void);
SymbolTable *new_symbol_table(void);
void free_symbol_table(SymbolTable *);
void free_symbol(Symbol *);

#endif /* SYMBOL_HANDLER_H */
