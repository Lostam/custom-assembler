#include "word_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"
#include "directive_handler.h"
#include "instruction_handler.h"
#include "logger.h"
#include "operation_handler.h"
#include "statement_handler.h"
#include "string_utils.h"
#include "symbol_handler.h"

int count_required_words(Statement *statement) {
    int count = 0;
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *instruction = new_instruction(statement);
        int number_of_operands = get_operands_number(instruction->operation);
        debug("Number of operands to operand [%d] is [%d]", instruction->operation, number_of_operands);
        // if both are registers, they will be written to the same word
        if (instruction->destination->type == REGISTER_MODE && instruction->source->type == REGISTER_MODE) {
            number_of_operands--;
        }
        free_instruction(instruction);
        count = 1 + number_of_operands;
    }
    else if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *directive = new_directive(statement);
        int count;
        if (directive->type == DIRECTIVE_TYPE_ENTRY || directive->type == DIRECTIVE_TYPE_EXTERNAL) {
            count = 0;
        }
        else if (directive->type == DIRECTIVE_TYPE_STRING) {
            // Not count both quotation marks and counting an empty letter
            count = strlen(directive->params) - 1;
        }
        else if (directive->type == DIRECTIVE_TYPE_DATA) {
            char delmiter[] = ", ";
            count = get_number_of_words(directive->params, delmiter);
        }
        free_directive(directive);
        
    }
    return count;
}

void add_words_to_assembler(Assembler *assembler, Statement *statement) {
    debug("Adding words for statement [%s]", statement->line);
    if (statement->type == STATEMENT_TYPE_DIRECTIVE) {
        Directive *dir = new_directive(statement);
        if (dir->type == DIRECTIVE_TYPE_DATA) {
            handle_data_directive(assembler, dir);
        }
        if (dir->type == DIRECTIVE_TYPE_STRING) {
            handle_string_directive(assembler, dir);
        }
        free_directive(dir);
    }
    else if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *inst = new_instruction(statement);
        handle_instruction(assembler, inst);
        free_instruction(inst);
    }
}

void handle_data_directive(Assembler *assembler, Directive *dir) {
    debug("Adding word for data directive [%s]", assembler->current_line);
    char *token;
    char *copy = strdup(dir->params);
    token = strtok(copy, ",");
    while (token != NULL) {
        Node *word = get_word_node(to_binary(atoi(trim_spaces(token)), 12));
        add_word(assembler, word);
        token = strtok(NULL, ", ");
    }
}

void handle_string_directive(Assembler *assembler, Directive *dir) {
    debug("Adding word for string directive [%s]", assembler->current_line);
    char *string = strdup(dir->params);
    string++;  // skipping first "
    while (*string != '"') {
        Node *word = get_word_node(to_binary((int)*string, 12));
        add_word(assembler, word);
        string++;
    }
    add_word(assembler, get_word_node(to_binary(0, 12)));
}

void handle_instruction(Assembler *assembler, Instruction *inst) {
    debug("Adding word for instruction [%s]", assembler->current_line);
    handle_operation(assembler, inst);
    handle_operands(assembler, inst);
}

void handle_operation(Assembler *assembler, Instruction *inst) {
    debug("Adding word for operation [%s]", assembler->current_line);
    Node *word = empty_word();
    strcat(word->content, to_binary(inst->source->type, 3));
    strcat(word->content, to_binary(inst->operation, 4));
    strcat(word->content, to_binary(inst->destination->type, 3));
    strcat(word->content, "00");
    add_word(assembler, word);
}

void handle_operands(Assembler *assembler, Instruction *inst) {
    Operand *source = inst->source;
    Operand *destination = inst->destination;
    if (source->type == UNDEFINED_MODE && destination->type == UNDEFINED_MODE) {
        return;
    }
    if (destination->type == REGISTER_MODE && source->type == REGISTER_MODE) {
        Node *single_word = empty_word();
        strcat(single_word->content, to_binary(atoi(&source->value[2]), 5));
        strcat(single_word->content, to_binary(atoi(&destination->value[2]), 5));
        strcat(single_word->content, "00");
        single_word->content[12] = '\0';
        add_word(assembler, single_word);
        return;
    }
    if (source->type != UNDEFINED_MODE) {
        add_operand_word(assembler, source);
    }
    add_operand_word(assembler, destination);
    return;
}

void add_operand_word(Assembler *assembler ,Operand *operand) {
    Node *word = empty_word();
        if (operand->type == IMMEDIATE_MODE) {
        word = get_immediate_operand_word(assembler, operand);
    }
    if (operand->type == DIRECT_MODE) {
        word = get_direct_operand_word(assembler, operand);
    }
    if (operand->type == REGISTER_MODE) {
        word = get_register_operand_word(assembler, operand);
    }
    if (word != NULL) {
        word->content[12] = '\0';
        add_word(assembler, word);
    }
}

Node *get_direct_operand_word(Assembler *assembler, Operand *operand) {
    Symbol *symbol = get_symbol_by_name(assembler->symbol_table, operand->value);
    if (symbol == NULL) {
        add_error(assembler, "No matching symbol for symbol [%s] in line [%d]", operand->value, assembler->current_line_number);
        return NULL;
    }
    Node *word = empty_word();
    debug("Symbol [%s] found in line %d", symbol->name, symbol->line_number);
    strcat(word->content, to_binary(symbol->line_number, 10));
    if (symbol->type == SYMBOL_TYPE_EXTERNAL) {
        strcat(word->content, "01");
    } else {
        strcat(word->content, "10");
    }
    return word;
}

Node *get_immediate_operand_word(Assembler *assembler, Operand *operand) {
    Node *word = empty_word();
    strcat(word->content, to_binary(atoi(operand->value), 10));
    strcat(word->content, "00");
    return word;
}

Node *get_register_operand_word(Assembler *assembler, Operand *operand) {
    Node *word = empty_word();
    if (operand->pos == DESTINATION) {
        strcat(word->content, to_binary(atoi(&operand->value[2]), 5));
        strcat(word->content, "00000");
    } else {
        strcat(word->content, "00000");
        strcat(word->content, to_binary(atoi(&operand->value[2]), 5));
    }

    strcat(word->content, "00");
    return word;
}

Node *empty_word() {
    Node *word = (Node *)malloc(sizeof(Node));
    if (word == NULL) {
        error("Memory allocation failed.\n");   
        exit(1);
    }
    word->content = (char *)malloc(13);
    if (word->content == NULL) {
        error("Memory allocation failed.\n");   
        exit(1);
    }
    word->next = NULL;
    return word;
}

Node *get_word_node(char data[13]) {
    Node *word = empty_word();
    strncpy(word->content, data, 12);
    word->content[12] = '\0';
    return word;
}

void add_word(Assembler *assembler, Node *word) {
    add_node_to_list(assembler->words_l_list, word);
    debug("Added word %s", assembler->words_l_list->tail->content);
}

char *to_binary(unsigned int num, unsigned int length) {
    char *binary = (char *)malloc((length + 1) * sizeof(char));
    if (binary == NULL) {
        error("Memory allocation failed!\n");
        exit(1);
    }

    binary[length] = '\0';

    for (int i = length - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return binary;
}
