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

enum op_adrs_mod_s { IMMEDIATE_MODE = 1, // a number
                     DIRECT_MODE = 3, // a label
                     REGISTER_MODE = 5, // a register (r@[0-7])
                     UNDEFINED_MODE = 0 };

enum op_adrs_pos_s { SOURCE,
                     DESTINATION };

struct operand_s {
    char *value; /**< The operand content */
    OpAddressMode type; /**< The type of the opernad, immediate,direct or register */
    OpAddressPosition pos; /**< either a source or destination */
};


/**
 * @brief key-value mapping of string value to operation enum
*/
struct operand_map_s {
    char *str; 
    OperationType op; 
};

/**
 * @brief key-value mapping of an operation enum to it allowed operands
*/
struct op_allowed_opr_s {
    OperationType op;
    OpAddressMode source[3];
    OpAddressMode destination[3];
};

/**
    Converts a string to an enum 
*/
OperationType string_to_operation(const char *);
/**
    @brief Converts an enum to string, used for logging
*/
const char *operation_to_string(OperationType);
/**
    @brief Converts an enum to string, used for logging
*/
const char *operation_mode_to_string(OpAddressMode);
/**
    @brief Builds and adds the operand to the passed operand 
*/
void add_operand_values(Operand *, const char *);
/**
    @brief Returns the number of operands an operation supports
*/
int get_operands_number(OperationType);
/**
    @brief Returns an array of allowed source and destination operands for an operation
*/
OperationAllowdOperands *get_allowed_operand(OperationType);
int is_valid_operation_name(const char *);
/**
    @brief Validates an operand is valid
*/
void validate_operands(Assembler *, OperationType, Operand *);
/**
    @brief Check if an operand is a valid operand for the operation
*/
int is_address_in_array(OpAddressMode[], OpAddressMode);
/**
    @brief Checks if a string is a register
*/
int is_register(const char *);
OpAddressMode get_address_mode(const char *);
Operand *new_empty_operand(OpAddressPosition);
void free_operand(Operand *);

#endif /** OPERATION_UTILS_H */
