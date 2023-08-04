#ifndef OPERATION_UTILS_H
#define OPERATION_UTILS_H

typedef enum op_adrs_mod_s OpAddressMode;
typedef enum op_adrs_pos_s OpAddressPosition;
typedef struct op_allowed_opr_s OperationAllowdOperands;
typedef struct operand_map_s OpeationMap;
typedef struct operand_s Operand;
typedef enum operand_type_s OperationType;
enum operand_type_s {
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
    stop,
    invalid_op = -1,
};

#include "assembler.h"
#include <string.h>

// todo :: change to capital letters
enum op_adrs_mod_s { Immediate = 1, // a number
                     Direct = 3, // a label
                     Register = 5, // a register (r@[0-7])
                     Undefined = 0 };

enum op_adrs_pos_s { SOURCE,
                     DESTINATION };

struct operand_s {
    const char *value;
    OpAddressMode type;
    OpAddressPosition pos;
};


struct operand_map_s {
    const char *str;
    OperationType op;
};

struct op_allowed_opr_s {
    OperationType op;
    OpAddressMode source[3];
    OpAddressMode destination[3];
};

OperationType string_to_operation(const char *);
const char *operation_to_string(OperationType);
// Operand *new_operand(const char *, OpAddressPosition); // todo :: add
Operand *new_empty_operand(OpAddressPosition);
void add_operand_values(Operand *, const char *);
int is_valid_operation_name(const char *);
void validate_operands(Assembler *, OperationType, Operand *);
int is_address_in_array(OpAddressMode[], OpAddressMode);
int get_operands_number(OperationType);
int is_register(const char *);
OpAddressMode get_address_mode(const char *);
OperationAllowdOperands *get_allowed_operand(OperationType);
// todo :: add free_operand
#endif /* OPERATION_UTILS_H */
