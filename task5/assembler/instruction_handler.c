#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "statementer.h"
#include "instruction_handler.h"
#include "assembler.h"
#include "logger.h"
#include "string_utils.h"

// #define VALID_DIRECTIVES ".data, .string, .entry, .extern"

// assume the structure of the line is : "OPTIONAL_LABEL: .<SOME_STRING>..."
void validate_instruction(Assembler *assembler, Statement *statement)
{   
    validate_max_number_of_words(*assembler, *statement);
    Instruction *instruction = new_instruction(statement);
    if (instruction->type == -1)
    {
        // fixme
        error("Found invalid instruction %s, valid instructions are %s", instruction->type_str, VALID_DIRECTIVES);
    }
    if (instruction->type == DATA)
    {
        info("Validating type %s\n", instruction->type_str);
        validate_data_params(assembler, instruction);
        info("Validated %s\n", statement->line);
    }
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

char *set_operands(Statement *statement, Instruction *instruction)
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
    instruction->destintion = new_address_op(token);
    token = strtok(NULL, " ");
    if (token == NULL) {
        error("Expected maximum of 2 words but found 3");
        return; // todo :: add error here
    }

}

// todo move to another class
AddressOp *new_address_op(char *operand) {
    AddressOp *addressOp = (AddressOp *)malloc(sizeof(AddressOp));

}

void validate_max_number_of_words(Assembler *assembler, Statement *statement) {
    char *line = get_command_line(statement);
    int count = get_number_of_words(line);
    if (count > 3) {
        add_error(assembler, "Expecting maximum of 2 opearnd for instruction command, got %d", count);
    }
}