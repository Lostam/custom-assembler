#include "symbol_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "directive_handler.h"
#include "logger.h"
#include "statement_handler.h"
#include "string_utils.h"

#define INITIAL_SIZE 10

// create a mapping the string representation of the operation to the operation enum, also write a method which takes as parameter a string and returns the operation enum

void collect_symbol(Assembler *assembler, Statement *statement) {
    // todo :: handle this later --> check if is declared for intern or extern directive.

    Symbol *symbol = get_symbol_from_statement(statement, assembler->current_line_number);
    if (symbol == NULL) {
        return;
    }
    if (is_symbol_in_map(assembler->symbol_table, statement->symbol)) {
        // log symbol already declared
        return;
    }
    add_to_table(assembler->symbol_table, symbol);
}

int is_valid_symbol() {
    // is_declerad_correctly() {

    // }
}

int is_symbol_in_map(SymbolTable *map, char *symbol) {
    int i = 0;
    for (i = 0; i < map->size; i++) {
        if (strncmp(map->table[i], symbol)) {
        }
    }
}

Symbol *get_symbol_from_statement(Statement *statement, int line_number) {
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        if (directive->type == DIRECTIVE_TYPE_ENTRY) {
            free(symbol);
            return NULL;
        }
        if (directive->type == DIRECTIVE_TYPE_EXTERNAL) {
            symbol->type = SYMBOL_TYPE_EXTERNAL;
            symbol->name = directive->params;
            return symbol;
        }
    }
    if (!is_empty(statement->symbol)) {
        symbol->line_number = line_number;
        symbol->type = SYMBOL_TYPE_RELOCATABLE;
        symbol->name = statement->symbol;
        if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
            symbol->sign = SYMBOL_SIGN_DATA;
        }
        if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
            symbol->sign = SYMBOL_SIGN_CODE;
        }
        return symbol;
    }
    free(symbol);
    return NULL;
}

void free_symbol_table(SymbolTable *table) {
    for (int i = 0; i < table->size; i++) {
        free(table->symbols[i]);
    }
    free(table->symbols);
    table->size = 0;
    table->capacity = 0;
    free(table);
}

void init_symbol_table() {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->symbols = (Symbol **)malloc(INITIAL_SIZE * sizeof(Symbol *));
    table->size = 0;
    table->capacity = INITIAL_SIZE;
    return table;
}

void add_to_table(SymbolTable *table, Symbol *symbol) {
    if (table->size == table->capacity) {
        // Double the capacity // todo :: maybe just increase by 10
        table->capacity *= 2;
        table->symbols = (Symbol *)realloc(table->symbols, table->capacity * sizeof(Symbol));
    }

    // Add the new item to the symbol_table
    table->symbols[table->size++] = symbol;
}


 // fixme :: extern and intern can support mutliple symbols