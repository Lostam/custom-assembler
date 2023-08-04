#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "file_handler.h"
#include "macro_handler.h"
#include "file_builder.h"

int clear_if_error(Assembler *);

int main(int argc, char *argv[]) {
    // use initializtion method
    for (int i = 1; i < argc; i++) {
        info("---------------Starting File %s---------------", argv[i]);
        Assembler *assembler = new_assembler(argv[i]);
        info("---------------Starting Macro Parsing---------------");
        parse_macros(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        set_open_file(assembler);
        info("---------------Starting Syntax Validation---------------");
        syntax_validation(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting First Data Collection---------------");
        data_collection(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting Second Data Collection---------------");
        build_data_for_files(assembler);
        if (clear_if_error(assembler)) {
            continue;
        }
        info("---------------Starting File Building---------------");
        generate_output_files(assembler);
        // stop if error
        // stop if error
        // build_data_for_files(assembler);

        fclose(assembler->as_file);

        return 0;
    }
}

int clear_if_error(Assembler *assembler) {
    if (assembler->has_error) {
        error("Found errors are : \n %s\n", assembler->error);
        free(assembler);
        return 1;
    }
    return 0;
}