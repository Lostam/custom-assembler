#include "operation_handler.h"

#include <ctype.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "macros.h"
#include "string_utils.h"

// todo :: move to constants file
OpeationMap operation_mapping[] = {
    {"mov", mov},
    {"cmp", cmp},
    {"add", add},
    {"sub", sub},
    {"not", not },
    {"clr", clr},
    {"lea", lea},
    {"inc", inc},
    {"dec", dec},
    {"jmp", jmp},
    {"bne", bne},
    {"red", red},
    {"prn", prn},
    {"jsr", jsr},
    {"rts", rts},
    {"stop", stop},
};

// todo :: change name
// todo :: move to constants file

OperationAllowdOperands allowd_operands_mapping[] = {
    {mov, {Immediate, Direct, Register}, {Direct, Register}},
    {cmp, {Immediate, Direct, Register}, {Immediate, Direct, Register}},
    {add, {Immediate, Direct, Register}, {Direct, Register}},
    {sub, {Immediate, Direct, Register}, {Direct, Register}},
    {not, {Undefined}, {Direct, Register}},
    {clr, {Undefined}, {Direct, Register}},
    {lea, {Register}, {Direct, Register}},
    {inc, {Undefined}, {Direct, Register}},
    {dec, {Undefined}, {Direct, Register}},
    {jmp, {Undefined}, {Direct, Register}},
    {bne, {Undefined}, {Direct, Register}},
    {red, {Undefined}, {Direct, Register}},
    {prn, {Undefined}, {Immediate, Direct, Register}},
    {jsr, {Undefined}, {Direct, Register}},
    {rts, {Undefined}, {Undefined}},
    {stop, {Undefined}, {Undefined}},
};

OperationType string_to_operation(const char *op_str) {
    for (int i = 0; i < sizeof(operation_mapping) / sizeof(OpeationMap); i++) {
        if (strcmp(operation_mapping[i].str, op_str) == 0) {
            return operation_mapping[i].op;
        }
    }
    return invalid_op;
}

const char *operation_to_string(OperationType operation) {
    for (int i = 0; i < sizeof(operation_mapping) / sizeof(OpeationMap); i++) {
        if (operation_mapping[i].op == operation) {
            return operation_mapping[i].str;
        }
    }
    return "";
}

int is_valid_operation_name(const char *op_str) {
    OperationType op = string_to_operation(op_str);
    if (op == invalid_op) {
        return 0;
    }
    return 1;
}

void validate_operands(Assembler *assembler, OperationType operation, Operand *operand) {
    OperationAllowdOperands *allowed_operands = get_allowed_operand(operation);
    if (allowed_operands != NULL) {
        OpAddressMode *arr;
        const char *name;
        if (operand->pos == DESTINATION) {
            arr = allowed_operands->destination;
            name = "destination";
        }
        if (operand->pos == SOURCE) {
            arr = allowed_operands->source;
            name = "source";
        }
        
        if (!is_address_in_array(arr, operand->type)) {
            add_error(assembler, "Operation [%s] has an invalid [%s] operand [%s], the operation is not supporting address mode %d\n",
         operation_to_string(operation), name, operand->value, operand->type);
        }
        return;
    }
}

// fixme :: should use pointer?
int is_address_in_array(OpAddressMode array[], OpAddressMode target) {
    size_t size = ARRAY_SIZE(array);  // fixme
    for (size_t i = 0; i <= size; i++) {
        if (array[i] == target) {
            return 1;
        }
    }
    return 0;
}

OperationAllowdOperands *get_allowed_operand(OperationType operation) {
    for (int i = 0; i < sizeof(allowd_operands_mapping) / sizeof(OperationAllowdOperands); i++) {
        if (allowd_operands_mapping[i].op == operation) {
            return &allowd_operands_mapping[i];
        }
    }
    return NULL;
}
// todo :: change me to count not undefined
int get_operands_number(OperationType operation) {
    OperationAllowdOperands *allowed_operands = get_allowed_operand(operation);
    if (allowed_operands == NULL) {
        return -1;
    }

    int is_dest_empty = allowed_operands->destination[0] == Undefined;
    int is_src_empty = allowed_operands->source[0] == Undefined;

    if (!is_src_empty && !is_dest_empty) {
        return 2;
    } else if (!is_src_empty || !is_dest_empty) {
        return 1;
    } else {
        return 0;
    }
}

// todo :: make sure it is freeded later
// Operand *new_operand(const char *operand, OpAddressPosition pos) {
//     Operand *addressOp = (Operand *)malloc(sizeof(Operand));
//     addressOp->value = operand;
//     addressOp->type = get_address_mode(operand);
//     addressOp->pos = pos;
//     return addressOp;
// }

Operand *new_empty_operand(OpAddressPosition pos) {
    Operand *operand = (Operand *)malloc(sizeof(Operand));
    operand->pos = pos;
    operand->type = Undefined;
    operand->value = "\0";
    return operand;
}

void add_operand_values(Operand *operand, const char *value) {
    operand->value = value;
    operand->type = get_address_mode(value);
    debug("Address mode for operand [%s] is : %d", value, operand->type);
}

OpAddressMode get_address_mode(const char *operand) {
    if (is_empty(operand)) {
        return Undefined;
    }
    if (is_register(operand)) {
        return Register;
    }
    if (is_whole_number(operand)) {
        return Immediate;
    }
    return Direct;
}

int is_register(const char *operand) {
    char *copy = strdup(operand);
    if (strlen(copy) != 3) {
        return 0;
    }
    if (strncmp(copy, "@r", 2)) {
        return 0;
    }
    if (!isdigit(copy[2]) || 0 > atoi(&copy[2]) || atoi(&copy[2]) > 7) {
        return 0;
    }
    return 1;
}

// todo :: add free operand