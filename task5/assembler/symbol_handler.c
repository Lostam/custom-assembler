#include "symbol_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "directive_handler.h"
#include "logger.h"
#include "macros.h"
#include "statement_handler.h"
#include "string_utils.h"

#define INITIAL_SIZE 10

// create a mapping the string representation of the operation to the operation enum, also write a method which takes as parameter a string and returns the operation enum
void collect_symbols_from_statement(Assembler *assembler, Statement *statement) {
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        if (directive->type == DIRECTIVE_TYPE_ENTRY) {
            free_directive(directive);
            return;
        }
        if (directive->type == DIRECTIVE_TYPE_EXTERNAL) {
            add_external_symbols(assembler, directive);
            free_directive(directive);
            return;
        }
        free_directive(directive);
    }
    if (!is_empty(statement->symbol)) {
        add_regular_symbol(assembler, statement);
    }
}

void add_external_symbols(Assembler *assembler, Directive *directive) {
    char *input = strdup(directive->params);
    char *token = strtok(input, ",");
    while (token != NULL) {
        token = trim_spaces(token);
        Symbol *symbol = new_empty_symbol();
        symbol->type = SYMBOL_TYPE_EXTERNAL;
        symbol->line_number = 0;
        symbol->name = strdup(token);
        symbol->used_at = new_linked_list();
        add_to_table(assembler, symbol);
        token = strtok(NULL, ",");
    }
    free(input);
}

void add_regular_symbol(Assembler *assembler, Statement *statement) {
    Symbol *symbol = new_empty_symbol();
    symbol->type = SYMBOL_TYPE_RELOCATABLE;
    symbol->name = strdup(statement->symbol);
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        symbol->sign = SYMBOL_SIGN_DATA;
        symbol->line_number = assembler->DC + BASE_MEMORY_COUNTER;
    }
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        symbol->sign = SYMBOL_SIGN_CODE;
        symbol->line_number = assembler->IC + BASE_MEMORY_COUNTER;
    }
    add_to_table(assembler, symbol);
}

Symbol *new_empty_symbol() {
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    if (symbol == NULL) {
        error("Memory allocation error");
        exit(1);
    }
    symbol->used_at = NULL;
    symbol->is_entry = 0;
    return symbol;
}

void add_counter_to_data(Assembler *assembler) {
    int i;
    for (i = 0; i < assembler->symbol_table->size; i++) {
        if (assembler->symbol_table->symbols[i]->sign == SYMBOL_SIGN_DATA) {
            assembler->symbol_table->symbols[i]->line_number += assembler->IC;
        }
    }
}

SymbolTable *new_symbol_table() {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    table->symbols = (Symbol **)malloc(INITIAL_SIZE * sizeof(Symbol *));
    if (table->symbols == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    table->size = 0;
    table->capacity = INITIAL_SIZE;
    return table;
}

void add_to_table(Assembler *assemebler, Symbol *symbol) {
    SymbolTable *table = assemebler->symbol_table;
    if (is_symbol_in_map(table, symbol)) {
        add_error(assemebler, "Symbol %s was already declared in file", symbol->name);
        return;
    }
    if (table->size == table->capacity) {
        table->capacity += INITIAL_SIZE;
        Symbol **temp = (Symbol **)realloc(table->symbols, table->capacity * sizeof(Symbol));
        if (temp == NULL) {
            error("Memory allocation failed!");
            exit(1);
        }
        table->symbols = temp;
    }
    table->symbols[table->size++] = symbol;
}

int is_symbol_in_map(SymbolTable *map, Symbol *symbol) {
    Symbol *found = get_symbol_by_name(map, symbol->name);
    return found != NULL;
}

Symbol *get_symbol_by_name(SymbolTable *map, const char *str) {
    if (map->size == 0) {
        return NULL;
    }
    int i = 0;
    for (i = 0; i < map->size; i++) {
        debug("Current symbol is : [%s] and looking for [%s]", map->symbols[i]->name, str);
        if (strcmp(str, map->symbols[i]->name) == 0) {
            return map->symbols[i];
        }
    }
    debug("Symbol not found");
    return NULL;
}

void validate_label(Assembler *assembler, const char *label) {
    size_t len = strlen(label);

    if (len < 1) {
        add_error(assembler, "Label [%s] is shorter then the minimum allowed of 1 letter, actual length is [%d]", label, len);
    }

    if (len > 31) {
        add_error(assembler, "Label [%s] is longer then the maximum allowed of 32, actual length is [%d]", label, len);
    }

    if (!isalpha(label[0])) {
        add_error(assembler, "Label [%s] first letter must be alphabetical, got [%c]", label, label[0]);
    }

    for (size_t i = 1; i < len; i++) {
        if (!isalnum(label[i])) {
            add_error(assembler, "Label letters must be alphanumerical, got [%c] for label [%s] in index [%d]", label[i], label, i);
        }
    }
    if (is_reserved_word(label)) {
        debug("Label [%s] is a reserved word", label);
    }
    return;
}

void free_symbol_table(SymbolTable *table) {
    for (int i = 0; i < table->size; i++) {
        free_symbol(table->symbols[i]);
    }
    free(table->symbols);
    table->size = 0;
    table->capacity = 0;
    free(table);
}

void free_symbol(Symbol *symbol) {
    free(symbol->name);
    if (symbol->used_at != NULL) {
        free_linked_list(symbol->used_at);
    }
    free(symbol);
}