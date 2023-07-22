#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statement_handler.h"
#include "assembler.h"
#include "symbol_handler.h"
#include "logger.h"

int MAX_LINE_LENGTH = 100;
int should_read_sentence(char*);

/**
 * This method is doing a syntax validation of the file
*/
void syntax_validation(Assembler *assembler) {
    FILE* file = fopen("example.txt", "r");
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        assembler->current_line_number = line_number;
        validate_statement(assembler);
    }
    error("error is %s\n", assembler->error);
}

void collect_symbols(Assembler *assembler) {
    assembler->symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    
    FILE* file = fopen("example.txt", "r");
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        assembler->current_line = line;
        assembler->current_line_number = line_number;
        validate_statement(assembler);
    }
    error("error is %s\n", assembler->error);
}



/**
 * Todo :: add doc
*/
void add_error(Assembler *assembler, const char *unformatted_error, ...) {
    // log the line number
    // log the line content
    va_list args;
    va_start(args, unformatted_error);

    char error[100]; // todo :: check size
    vsnprintf(error, sizeof(error), unformatted_error, args);

    va_end(args);
    strcat(assembler->error , error);
}



//typedef struct operand {
//    char *name;
//    address_mode mode
//} Operand;
//
//static struct {
//	const char *decString;
//	labelType type;
//} decTab[] = {
//	{
//	".code", labelCode}, {
//	".data", labelData}, {
//	".extern", labelExtern}, {
//	".entry", labelEntry}, {
//".string", labelString},};
//
//typedef struct instruction {
//    operation op;
//    Operand source
//    Operand destination
//    int encoding
//} Instruction;
//
//
//operand read_op() {
//// if contains @r1-7 -> Register
//// if is an integer -> Immediate
//// otherwise -> Direct
//}
//
//// add code for reading instruction
// handle directive :
    // .data
    // .string
    // .entry
    // .extern

// שורת הוראה - שני סיביות ראשונות הן 0
// instruction sentence - ARE is always 0