#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statementer.h"
#include "assembler.h"
#include "logger.h"

int MAX_LINE_LENGTH = 100;
int should_read_sentence(char*);

int main(int argc, char *argv[]) {
    FILE* file = fopen("example.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    
    // use initializtion method
    Assembler *assembler = (Assembler*)malloc(sizeof(Assembler));
    assembler->file = file;
    assembler->iteration_step = SYNTAX_VALIDATION;
    // use constant
    assembler->error = malloc(200 * sizeof(char));
    syntax_validation(assembler);

    fclose(file);

    return 0;
}

/**
 * This method is doing a syntax validation of the file
*/
void syntax_validation(Assembler *assembler) {
    FILE* file = fopen("example.txt", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        validate_statement(assembler, line);
    }
    error("error is %s\n", assembler->error);
}

/**
 * Todo :: add doc
*/
void add_error(Assembler *assembler, const char *unformatted_error, ...) {
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