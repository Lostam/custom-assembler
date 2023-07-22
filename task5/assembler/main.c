#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statement_handler.h"
#include "assembler.h"
#include "logger.h"

int MAX_LINE_LENGTH = 100;

int main(int argc, char *argv[]) {
    FILE* file = fopen("example.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    
    // use initializtion method
    Assembler *assembler = (Assembler*)malloc(sizeof(Assembler));
    assembler->current_file = file;
    assembler->iteration_step = SYNTAX_VALIDATION;
    // use constant
    assembler->error = malloc(200 * sizeof(char));
    syntax_validation(assembler);
    // stop if error
    collect_symbols(assembler);
    // stop if error
    // stop if error
    build_files(assembler);

    fclose(file);

    return 0;
}