#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "assembler.h"
#include "string_utils.h"
#include "operation_handler.h"
#include "statement_handler.h"

typedef enum {  Immediate = 1, Direct = 3, Register = 5, Undefined = -1 } OpAddressMode ;


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
    AddressOp *destination;
} Instruction;

void validate_instruction(Assembler *, Statement *);
Instruction *new_instruction(Statement *);
char *get_operation_name(Statement *);
char *get_instruction_params(Statement *);
void set_operands(Statement *, Instruction *);
AddressOp *new_address_op(char *);
OpAddressMode get_address_mode(const char *);
int is_register(const char *);
void validate_max_number_of_words(Assembler *, Statement *);
void free_instruction(Instruction *)



#endif /* INSTRUCTION_HANDLER_H */

// todo :: check for all usage of strcpy and check if need to free