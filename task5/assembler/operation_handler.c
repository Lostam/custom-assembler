#include <string.h>
#include "operation_handler.h"
#include "logger.h"

OpeationMap operation_mapping[] = {
    {"mov", mov},
    {"cmp", cmp},
    {"add", add},
    {"sub", sub},
    {"not", not},
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

OperationType string_to_operation(const char *op_str) {
    for(int i = 0; i < sizeof(operation_mapping) / sizeof(OpeationMap); i++) {
        if(strcmp(operation_mapping[i].str, op_str) == 0) {
            return operation_mapping[i].op;
        }
    }
    return -1;
}

int is_valid_operand_name(const char *op_str) {
    OperationType op = string_to_operation(op_str);
    if (op == -1) {
        return 0;
    }
    return 1;
}