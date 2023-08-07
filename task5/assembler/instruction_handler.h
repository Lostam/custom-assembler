#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

typedef struct instruction_s Instruction;

#include "assembler.h"
#include "operation_handler.h"
#include "statement_handler.h"
#include "string_utils.h"

struct instruction_s {
    char *symbol; /**< The instruction symbol if exists */
    char *operation_string; /**< The operation as string, used for logging */
    OperationType operation; /**< The instruction's operation enum */
    Operand *source; /**< The instruction source operand if exists */
    Operand *destination; /**< The instruction destination operand if exists */
};

char *get_instruction_params(Statement *);
/**
    @brief an instruction, will set it operand
*/
void set_operands(Statement *, Instruction *);
/**
    @brief Validates the allowed operand for an operation
*/
OperationAllowdOperands *get_allowed_operand(OperationType);
/**
    @brief Returns the operation name from a statement 
*/
char *get_operation_name(Statement *);
/**
    @brief Validates an instruction has no more then 2 parameters
*/
void validate_max_number_of_params_for_instruction(Assembler *, Statement *);

void validate_instruction(Assembler *, Statement *);
Instruction *new_instruction(Statement *);
void free_instruction(Instruction *);

#endif /* INSTRUCTION_HANDLER_H */