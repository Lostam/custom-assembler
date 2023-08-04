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
    Symbol *symbol = get_symbol_from_statement(statement, assembler->IC, assembler->DC);
    if (symbol == NULL) {
        return;
    }
    // todo :: add IC only after finishing counting all :: page  41
    add_to_table(assembler, symbol);
}

/*
    Adding IC to all the data symbols
*/
void add_counter_to_data(Assembler *assembler) {
    int i;
    for (i = 0; i < assembler->symbol_table->size; i++) {
        if (assembler->symbol_table->symbols[i]->sign == SYMBOL_SIGN_DATA) {
            assembler->symbol_table->symbols[i]->line_number += assembler->IC;
        }
    }
}

int is_valid_symbol() {
    // is_declerad_correctly() {

    // }
    return 1;
}

// todo :: better way to do it?
Symbol *get_symbol_from_statement(Statement *statement, int IC, int DC) {
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    symbol->is_entry = 0;
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        if (directive->type == DIRECTIVE_TYPE_ENTRY) {
            free_symbol(symbol);
            return NULL;
        }
        if (directive->type == DIRECTIVE_TYPE_EXTERNAL) {
            symbol->type = SYMBOL_TYPE_EXTERNAL;
            symbol->line_number = 0;
            symbol->name = directive->params;
            symbol->used_at = new_linked_list();
            return symbol;
        }
    }
    if (!is_empty(statement->symbol)) {
        symbol->type = SYMBOL_TYPE_RELOCATABLE;
        symbol->name = statement->symbol;
        if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
            symbol->sign = SYMBOL_SIGN_DATA;
            symbol->line_number = DC + 100;
        }
        if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
            symbol->sign = SYMBOL_SIGN_CODE;
            symbol->line_number = IC + 100;
        }
        return symbol;
    }
    free_symbol(symbol);
    return NULL;
}


SymbolTable *new_symbol_table() {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->symbols = (Symbol **)malloc(INITIAL_SIZE * sizeof(Symbol *));
    table->size = 0;
    table->capacity = INITIAL_SIZE;
    return table;
}

void add_to_table(Assembler *assemebler, Symbol *symbol) {
    SymbolTable *table = assemebler->symbol_table;
    if (is_symbol_in_map(table, symbol)) {
        printf("Symbol %s was already declared in line %d", symbol->name, symbol->line_number);
        add_error(assemebler, "Symbol %s was already declared in line %d", symbol->name, symbol->line_number);
        return;
    }
    if (table->size == table->capacity) {
        // todo :: maybe just increase by 10
        table->capacity *= 2;
        table->symbols = (Symbol **)realloc(table->symbols, table->capacity * sizeof(Symbol));
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
        if (strcmp(str, map->symbols[i]->name) == 0) {
            return map->symbols[i];
        }
    }
    return NULL;
}

// todo :: add logs
int is_valid_label(const char *label) {
    // todo test for reserved words
    size_t len = strlen(label);

    if (len < 1 || len > 31) {
        debug("too long");
        return 0;
    }

    if (!isalpha(label[0])) {
        debug("first lettr not alpha");
        return 0;
    }

    for (size_t i = 1; i < len; i++) {
        if (!isalnum(label[i])) {
            debug("letter [%c] is lettr not alpha", label[i]);
            return 0;
        }
    }
    return 1;
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

void free_symbol(Symbol *symbol) {
    free(symbol->name);
    free(symbol->used_at);
    free(symbol);
}

// fixme :: extern and intern can support mutliple symbols

// unsigned hash(char *s) {
//     unsigned hashval;
//     for (hashval = 0; *s != '\0'; s++) {
//         hashval = *s + 31 * hashval;
//     }
//     return hashval % HASHSIZE;
// }

// struct nlist *lookup(char *s) {
//     struct nlist *np;
//     for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
//         if (strcmp(s, np->name) == 0) {
//             return np;
//         }
//     }
//     return NULL; /* not found */
// }