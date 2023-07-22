#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "statement_handler.h"
#include "directive_handler.h"
#include "instruction_handler.h"
#include "string_utils.h"
#include "operation_handler.h"


// TODO :: test methods

// should be only called once statement was validated
int count_required_words(Statement *statement) {
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *instruction = new_instruction(statement);
        int number_of_operands = get_operands_number(instruction);
    
        // if both are registers, they will be written to the same word
        if (instruction->destination->type == Register && instruction->source->type == Register) {
            number_of_operands--;
        }
        return 1 + number_of_operands;
        free_instruction(instruction);
    }
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        if (directive->type == DIRECTIVE_TYPE_ENTRY || directive->type == DIRECTIVE_TYPE_EXTERNAL) {
            return 0;
        }
        if (directive->type == DIRECTIVE_TYPE_STRING) { 
            // Not count both quotation marks and counting an empty letter
            return strlen(directive->params) - 1 ;
        }
        if (directive->type == DIRECTIVE_TYPE_DATA) { 
            return get_number_of_words(directive->params, ',');
        }
        free_directive(directive);
    }
    return 0;
}

// int *get_words() {
    
// }