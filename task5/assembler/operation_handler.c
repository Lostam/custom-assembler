#include "operation_handler.h"

#include <ctype.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "macros.h"
#include "string_utils.h"

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

OperationAllowdOperands allowd_operands_mapping[] = {
    {mov, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {cmp, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}},
    {add, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {sub, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {not, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {clr, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {lea, {REGISTER_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {inc, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {dec, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {jmp, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {bne, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {red, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {prn, {UNDEFINED_MODE}, {IMMEDIATE_MODE, DIRECT_MODE, REGISTER_MODE}},
    {jsr, {UNDEFINED_MODE}, {DIRECT_MODE, REGISTER_MODE}},
    {rts, {UNDEFINED_MODE}, {UNDEFINED_MODE}},
    {stop, {UNDEFINED_MODE}, {UNDEFINED_MODE}},
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

// I'm using static here so I won't have to clear the string every time, it is used only for logging
const char *operation_mode_to_string(OpAddressMode address_mode) {
    static char str[10];
    if (address_mode == IMMEDIATE_MODE) {
        strcpy(str, "immediate");
    } else if (address_mode == DIRECT_MODE) {
        strcpy(str, "direct");
    } else if (address_mode == REGISTER_MODE) {
        strcpy(str, "register");
    } else if (address_mode == UNDEFINED_MODE) {
        strcpy(str, "empty");
    } else {
        strcpy(str, "");
    }

    return str;
}

int is_valid_operation_name(const char *op_str) {
    OperationType op = string_to_operation(op_str);
    if (op == invalid_op) {
        return 0;
    }
    return 1;
}

void validate_operands(Assembler *assembler, OperationType operation, Operand *operand) {
    if (operand->type == IMMEDIATE_MODE && atoll(operand->value) > MAXIMUM_ALLOWED_NUMBER) {
        add_error(assembler, "Invalid immediate operand [%s] : number [%s] is larger then the allowed %d",
                  assembler->current_line, operand->value, MAXIMUM_ALLOWED_NUMBER);
    }
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
            add_error(assembler, "Operation [%s] in line [%d] has an invalid [%s] operand [%s], the operation is not supporting address mode [%s]",
                      operation_to_string(operation), assembler->current_line_number, name, operand->value, operation_mode_to_string(operand->type));
        }

        return;
    }
}

int is_address_in_array(OpAddressMode array[], OpAddressMode target) {
    for (size_t i = 0; i < 3; i++) {
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
    if (allowed_operands == NULL) {
        return -1;
    }

    int is_dest_empty = allowed_operands->destination[0] == UNDEFINED_MODE;
    int is_src_empty = allowed_operands->source[0] == UNDEFINED_MODE;

    if (!is_src_empty && !is_dest_empty) {
        return 2;
    } else if (!is_src_empty || !is_dest_empty) {
        return 1;
    } else {
        return 0;
    }
}

Operand *new_empty_operand(OpAddressPosition pos) {
    Operand *operand = (Operand *)malloc(sizeof(Operand));
    if (operand == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    operand->pos = pos;
    operand->type = UNDEFINED_MODE;
    operand->value = NULL;
    return operand;
}

void add_operand_values(Operand *operand, const char *value) {
    operand->value = strdup(value);
    operand->type = get_address_mode(operand->value);
    debug("Address mode for operand [%s] is : [%s]", operand->value, operation_mode_to_string(operand->type));
}

OpAddressMode get_address_mode(const char *operand) {
    if (is_empty(operand)) {
        return UNDEFINED_MODE;
    }
    if (is_register(operand)) {
        return REGISTER_MODE;
    }
    if (is_whole_number(operand)) {
        return IMMEDIATE_MODE;
    }
    return DIRECT_MODE;
}

int is_register(const char *operand) {
    char *copy = strdup(operand);
    int result = 1;
    if (strlen(copy) != 3) {
        result = 0;
    }
    else if (strncmp(copy, "@r", 2)) {
        result = 0;
    }
    else if (!isdigit(copy[2]) || 0 > atoi(&copy[2]) || atoi(&copy[2]) > 7) {
        result = 0;
    }
    free(copy);
    return result;
}

void free_operand(Operand *operand) {
    if (operand->value != NULL) {
        free(operand->value);
    }
    free(operand);
}
