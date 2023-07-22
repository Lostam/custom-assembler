#ifndef OPERATION_UTILS_H
#define OPERATION_UTILS_H

#include <string.h>


typedef enum
{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
} OperationType;

typedef struct
{
    const char *str;
    OperationType op;
} OpeationMap;

typedef struct
{
    OperationType op;
    OpAddressMode source[3];
    OpAddressMode destination[3];
} OperationAllowdOperands;

#include "instruction_handler.h"
OperationType string_to_operation(const char *op_str);
int is_valid_operation_name(const char *op_str);
int validate_operands(Assembler *, Instruction *);
int is_address_in_array(const int *, size_t , AddressOp *);
int get_operands_number(OperationType);
#endif /* OPERATION_UTILS_H */
