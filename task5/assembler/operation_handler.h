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

OperationType string_to_operation(const char *op_str);
int is_valid_operand_name(const char *op_str);

#endif /* OPERATION_UTILS_H */
