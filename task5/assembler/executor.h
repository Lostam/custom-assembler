#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "statement_handler.h"


void first_data_collection(Assembler *);
void second_data_collection(Assembler *);
void mark_entry(Assembler *, Statement *);
void mark_external(Assembler *, Statement *);
void mark_external_if_needed(Assembler *, Operand *, int);
    
#endif /* EXECUTOR_H */


