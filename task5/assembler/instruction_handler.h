#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

typedef struct instruction_s Instruction;

#include "assembler.h"
#include "operation_handler.h"
#include "statement_handler.h"
#include "string_utils.h"

struct instruction_s {
    const char *symbol;
    const char *operation_string;
    OperationType operation;
    Operand *source;  // maybe use unsigned char instead
    Operand *destination;
};

void validate_instruction(Assembler *, Statement *);
Instruction *new_instruction(Statement *);
char *get_operation_name(Statement *);
char *get_instruction_params(Statement *);
void set_operands(Statement *, Instruction *);
void validate_max_number_of_words(Assembler *, Statement *);
void free_instruction(Instruction *);

#endif /* INSTRUCTION_HANDLER_H */

// todo :: check for all usage of strcpy and check if need to free