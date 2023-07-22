#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statement_handler.h"
#include "instruction_handler.h"
#include "assembler.h"
#include "logger.h"
#include "string_utils.h"

// #define VALID_DIRECTIVES ".data, .string, .entry, .extern"

// assume the structure of the line is : "OPTIONAL_LABEL: .<SOME_STRING>..."
void validate_instruction(Assembler *assembler, Statement *statement) {
    validate_max_number_of_words(assembler, statement);
    Instruction *instruction = new_instruction(statement);
    validate_operands(assembler, instruction);
    free(instruction->destintion);
    free(instruction->source);
    free(instruction);
}

Instruction *new_instruction(Statement *statement)
{
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    instruction->symbol = statement->symbol;
    instruction->operation_string = get_operation_name(statement);
    instruction->operation = string_to_operation(instruction->operation_string);
    set_operands(statement, instruction);

    return instruction;
}
char *get_operation_name(Statement *statement)
{
    char *line = get_command_line(statement);
    return strtok(line, " ");
}

char *get_instruction_params(Statement *statement)
{
    char *line = get_command_line(statement);
    info("Command line is : %s", line);
    // char *first_word = strtok(line, " "); // Extract the first word
    return removeFirstNWords(line, 1);
}

void set_operands(Statement *statement, Instruction *instruction)
{
    char *line = get_command_line(statement);
    char *params = removeFirstNWords(line, 1);
    info("Params line is : %s", params);
    instruction->destintion = NULL;
    instruction->source = NULL;
    char *token = strtok(params, " ");
    if (token == NULL) { 
        return;
    }
    instruction->destintion = new_address_op(token);
    token = strtok(NULL, " ");
    if (token == NULL) {
        return;
    }
    instruction->source = new_address_op(token);
}
//===================================================
// OP ADDRESS START
//===================================================
// todo move to another class
AddressOp *new_address_op(char *operand) {
    AddressOp *addressOp = (AddressOp *)malloc(sizeof(AddressOp));
    addressOp->value = operand;
    addressOp->type = get_address_mode(operand);
    return addressOp;
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
    if (strlen(copy)!=3) {
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

//===================================================
// OP ADDRESS END
//===================================================



void validate_max_number_of_words(Assembler *assembler, Statement *statement) {
    char *line = get_command_line(statement);
    int count = get_number_of_words(line);
    if (count > 3) {
        add_error(assembler, "Expecting maximum of 2 opearnd for instruction command, got %d", count);
    }
}