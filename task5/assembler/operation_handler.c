#include <string.h>
#include "operation_handler.h"
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
OperationAllowdOperands op_to_operand_mapping[] = {
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

OperationType string_to_operation(const char *op_str)
{
    for (int i = 0; i < sizeof(operation_mapping) / sizeof(OpeationMap); i++)
    {
        if (strcmp(operation_mapping[i].str, op_str) == 0)
        {
            return operation_mapping[i].op;
        }
    }
    return -1;
}

int is_valid_operation_name(const char *op_str)
{
    OperationType op = string_to_operation(op_str);
    if (op == -1)
    {
        return 0;
    }
    return 1;
}

int validate_operands(Assembler *assembler, Instruction *instruction)
{
    for (int i = 0; i < sizeof(op_to_operand_mapping) / sizeof(OperationAllowdOperands); i++)
    {
        if (op_to_operand_mapping[i].op == instruction->operation)
        {
            size_t size = sizeof(op_to_operand_mapping[i].destination) / sizeof(op_to_operand_mapping[i].destination[0]);
            if (!is_address_in_array(op_to_operand_mapping->destination, size, instruction->destintion)) {
                error("destintation is not in array");
            }

            size_t size2 = sizeof(op_to_operand_mapping[i].source) / sizeof(op_to_operand_mapping[i].source[0]);
            if (!is_address_in_array(op_to_operand_mapping->source, size, instruction->source)) {
                error("source is not in array");
            }
            // todo :: finish me
            // return op_to_operand_mapping[i].op;
        }
    }
    return -1;
}

// todo :: move it
int is_address_in_array(const int *array, size_t size, AddressOp *target)
{
    for (size_t i = 0; i < size; i++)
    {
        if (array[i] == target)
        {
            return 1;
        }
    }
    return 0;
}
