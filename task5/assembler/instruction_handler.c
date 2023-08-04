#include "instruction_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "statement_handler.h"
#include "string_utils.h"

// #define VALID_DIRECTIVES ".data, .string, .entry, .extern"
#define VALID_OPERATIONS "jmp...." // todo :: fill me

void validate_instruction(Assembler *assembler, Statement *statement) {
    validate_max_number_of_words(assembler, statement);
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
    debug("Adding word for instruction [%s]", statement->line);
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    instruction->symbol = statement->symbol;
    instruction->operation_string = get_operation_name(statement);
    instruction->operation = string_to_operation(instruction->operation_string);
    set_operands(statement, instruction);
    return instruction;
}
char *get_operation_name(Statement *statement) {
    char *line = get_command_line(statement);
    return strtok(line, " ");
}

char *get_instruction_params(Statement *statement) {
    char *line = get_command_line(statement);
    // char *first_word = strtok(line, " "); // Extract the first word
    return remove_first_n_words(line, 1);
}

void set_operands(Statement *statement, Instruction *instruction) {
    const char *line = get_command_line(statement);
    char *original_params = remove_first_n_words(line, 1);
    char *params = strdup(original_params);
    instruction->source = new_empty_operand(SOURCE);
    instruction->destination = new_empty_operand(DESTINATION);
    char *token = strtok(params, ",");
    if (token == NULL) {
        free(params);
        return;
    }
    add_operand_values(instruction->destination, trim_spaces(token));
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(params);
        return;
    }
    free(params);
    // switch
    add_operand_values(instruction->source, trim_spaces(token));
    // incase we found both destination and source, we need to switch their values
    // todo :: explain why
    Operand* temp = instruction->source;
    instruction->source = instruction->destination;
    instruction->destination = temp;
}
//===================================================
// OP ADDRESS START
//===================================================
// todo move to another class

//===================================================
// OP ADDRESS END
//===================================================

void validate_max_number_of_words(Assembler *assembler, Statement *statement) {
    char *line = get_command_line(statement); // fixme, not working correctly
    int count = get_number_of_words(line, ' ');
    if (count > 3) {
        // todo :: memory error probably
        add_error(assembler, "Expecting maximum of 2 operand for instruction command, got %d, original operands %s", count, line);
    }
}

void free_instruction(Instruction *instruction) {
    if (instruction == NULL) {
        return;  // Nothing to free
    }

    if (instruction->source != NULL) {
        free(instruction->source);
    }

    if (instruction->destination != NULL) {
        free(instruction->destination);
    }

    free(instruction);
}