#include "instruction_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "statement_handler.h"
#include "string_utils.h"

#define VALID_OPERATIONS "jmp mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop"

void validate_instruction(Assembler *assembler, Statement *statement) {
    validate_max_number_of_params_for_instruction(assembler, statement);
    Instruction *instruction = new_instruction(statement);
    if (instruction->operation == invalid_op) {
        add_error(assembler, "Operation %s is invalid, should use on of %s operations", instruction->operation_string, VALID_OPERATIONS);
    } else {
        validate_operands(assembler, instruction->operation, instruction->source);
        validate_operands(assembler, instruction->operation, instruction->destination);
    }

    free_instruction(instruction);
}

Instruction *new_instruction(Statement *statement) {
    debug("Creating a new instruction for word [%s]", statement->line);
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    if (instruction == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    instruction->symbol = NULL;
    if (statement->symbol != NULL) {
        instruction->symbol = strdup(statement->symbol);
    }
    instruction->operation_string = get_operation_name(statement);
    instruction->operation = string_to_operation(instruction->operation_string);
    set_operands(statement, instruction);
    debug("Operation was created with source [%s]", instruction->destination->value);
    return instruction;
}

char *get_operation_name(Statement *statement) {
    debug("Attempting to get operation name for statement [%s]", statement->line);
    char *line = get_command_line(statement);
    char *word = get_first_word(line);
    free(line);
    return word;
}

char *get_instruction_params(Statement *statement) {
    char *line = get_command_line(statement);
    char *params =  remove_first_n_words(line, 1);
    free(line);
    return params;
}

void set_operands(Statement *statement, Instruction *instruction) {
    debug("Attempting to set operands for statement [%s]", statement->line);
    char *line = get_command_line(statement);
    char *params = remove_first_n_words(line, 1);
    free(line);
    instruction->source = new_empty_operand(SOURCE);
    instruction->destination = new_empty_operand(DESTINATION);
    char *token = strtok(params, ",");
    if (token == NULL) {
        free(params);
        return;
    }
    token = trim_spaces(token);
    add_operand_values(instruction->destination, token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(params);
        return;
    }
    token = trim_spaces(token);
    add_operand_values(instruction->source, token);
    free(params);
    // incase we found both destination and source, we need to switch their values
    Operand* temp = instruction->source;
    instruction->source = instruction->destination;
    instruction->destination = temp;
}

void validate_max_number_of_params_for_instruction(Assembler *assembler, Statement *statement) {
    char *line = get_command_line(statement);
    char delimiters[] = ", ";
    int count = get_number_of_words(line, delimiters);
    free(line);
    if (count > 3) {
        add_error(assembler, "Expecting maximum of 2 operand for instruction command, got %d, original operands %s", count, line);
    }
}

void free_instruction(Instruction *instruction) {
    if (instruction == NULL) {
        return;
    }
    if (instruction->symbol != NULL) {
        free(instruction->symbol);
    }

    if (instruction->source != NULL) {
        free_operand(instruction->source);
    }

    if (instruction->destination != NULL) {
        free_operand(instruction->destination);
    }
    free(instruction->operation_string);
    free(instruction);
}