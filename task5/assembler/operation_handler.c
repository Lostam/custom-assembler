#include "operation_handler.h"

#include <string.h>

#include "logger.h"

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
    {not, {}, {Direct, Register}},
    {clr, {}, {Direct, Register}},
    {lea, {Register}, {Direct, Register}},
    {inc, {}, {Direct, Register}},
    {dec, {}, {Direct, Register}},
    {jmp, {}, {Direct, Register}},
    {bne, {}, {Direct, Register}},
    {red, {}, {Direct, Register}},
    {prn, {}, {Immediate, Direct, Register}},
    {jsr, {}, {Direct, Register}},
    {rts, {}, {}},
    {stop, {}, {}},
};

OperationType string_to_operation(const char *op_str) {
    for (int i = 0; i < sizeof(operation_mapping) / sizeof(OpeationMap); i++) {
        if (strcmp(operation_mapping[i].str, op_str) == 0) {
            return operation_mapping[i].op;
        }
    }
    return -1;
}

int is_valid_operation_name(const char *op_str) {
    OperationType op = string_to_operation(op_str);
    if (op == -1) {
        return 0;
    }
    return 1;
}

// fixme :: why not void?
int validate_operands(Assembler *assembler, Instruction *instruction) {
    OperationAllowdOperands *allowed_operands = get_allowed_operand(instruction->operation);
    if (allowed_operands != NULL) {
        size_t size = sizeof(allowd_operands_mapping[i].destination) / sizeof(allowd_operands_mapping[i].destination[0]);
        if (!is_address_in_array(allowd_operands_mapping->destination, size, instruction->destination)) {
            error("destintation is not in array");
        }

        size_t size2 = sizeof(allowd_operands_mapping[i].source) / sizeof(allowd_operands_mapping[i].source[0]);
        if (!is_address_in_array(allowd_operands_mapping->source, size, instruction->source)) {
            error("source is not in array");
        }
        // todo :: finish me
        // return op_to_operand_mapping[i].op;
    }
    return -1;
}

// todo :: move it
int is_address_in_array(const int *array, size_t size, AddressOp *target) {
    for (size_t i = 0; i < size; i++) {
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
int get_operands_number(OperationType operation) {
    OperationAllowdOperands *allowed_operands = get_allowed_operand(operation);
    if (allowed_operands != NULL) {
        return -1;
    }
    // todo :: consider extracting to method
    int is_dest_empty = sizeof(allowed_operands->destination) / sizeof(allowed_operands->destination[0]);
    int is_src_empty = sizeof(allowed_operands->source) / sizeof(allowed_operands->source[0]);

    if (!is_src_empty && !is_dest_empty) {
        return 2;
    } else if (!is_src_empty || !is_dest_empty) {
        return 1;
    } else {
        return 0;
    }
}

int is_array_empty(OpAddressMode arr[]) {
    for (int i = 0; i < 3; i++) {
        if (arr[i] != Undefined) {
            return false;
        }
    }
    return true;
}