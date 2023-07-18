#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "assembler.h"
#include "string_utils.h"
#include "operation_handler.h"

typedef enum { Direct = 1, Immediate = 3, Register = 5, Undefined = -1 } OpAddressMode ;


// Operand address encoding :
// Direct - 001
// Immediate - 011
// Register - 101

typedef struct
{
    const char *value;
    OpAddressMode type;
} AddressOp;

typedef struct
{
    const char *symbol;
    const char *operation_string;
    OperationType operation;
    AddressOp *source; // maybe use unsigned char instead
    AddressOp *destintion;
} Instruction;

void validate_instruction(Assembler *, Statement *);
Instruction *new_instruction(Statement *);
char *get_directive_name(Statement *);
char *get_directive_params(Statement *);



#endif /* INSTRUCTION_HANDLER_H */
