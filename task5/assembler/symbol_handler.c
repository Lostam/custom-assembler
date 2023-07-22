#include "statement_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "statement_handler.h"
#include "logger.h"
#include "string_utils.h"

// create a mapping the string representation of the operation to the operation enum, also write a method which takes as parameter a string and returns the operation enum

void collect_symbol(Assembler *assembler) {
    Statement *statement = new_statement(assembler->current_line);
    // todo :: handle this later --> check if is declared for intern or extern directive.
    if (!is_empty(statement->symbol)) {
        free(statement);
        return;
    }
    if (is_symbol_in_map(assembler->symbol_table, statement->symbol)) {

    }
    SymbolKV *kv = (SymbolKV *)malloc(sizeof(SymbolKV));
    add_to_table(assembler->symbol_table, kv);        
}


int is_valid_symbol() {

    
    // is_declerad_correctly() {

    // }
}

int is_symbol_in_map(SymbolTable *map, char *symbol) {
    int i=0;
    for (i = 0; i < map->size; i += 6) {
        if (strncmp(map->table[i], symbol))
    }
}

void add_to_table(SymbolTable *sym_table, SymbolKV *kv) {
    size_t size = sym_table->size;
    sym_table->table[size] = kv;
    sym_table->size++;
}