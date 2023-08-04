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


// should be only called once statement was validated
int count_required_words(Statement *statement) {
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
        Instruction *instruction = new_instruction(statement);
        int number_of_operands = get_operands_number(instruction->operation);
        debug("Number of operands to operand [%d] is [%d]", instruction->operation, number_of_operands);
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
            return strlen(directive->params) - 1;
        }
        if (directive->type == DIRECTIVE_TYPE_DATA) {
            return get_number_of_words(directive->params, ',');
        }
        free_directive(directive);
    }
    return 0;
}

void add_words_to_assembler(Assembler *assembler, Statement *statement) {
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
    if (statement->type == STATEMENT_TYPE_INSTRUCTION) {
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
    handle_operation(assembler, inst);
    handle_operands(assembler, inst);
}

void handle_operation(Assembler *assembler, Instruction *inst) {
    debug("Adding word for operation [%s]", assembler->current_line);
    Node *word = empty_word();
    // todo :: add ARE
    strcat(word->content, to_binary(inst->source->type, 3));
    strcat(word->content, to_binary(inst->operation, 4));
    strcat(word->content, to_binary(inst->destination->type, 3));
    strcat(word->content, "00");
    add_word(assembler, word);
}

// todo :: break to smaller methods
void handle_operands(Assembler *assembler, Instruction *inst) {
    if (inst->source->type == Undefined && inst->destination->type == Undefined) {
        return;
    }
    if (inst->destination->type == Register && inst->source->type == Register) {
        Node *single_word = empty_word();
        strcat(single_word->content, to_binary(atoi(&inst->source->value[2]), 5));
        strcat(single_word->content, to_binary(atoi(&inst->destination->value[2]), 5));
        strcat(single_word->content, "00");
        single_word->content[12] = '\0';
        add_word(assembler, single_word);
        return;
    }
    if (inst->source->type != Undefined) {
        Node *source_word = NULL;
        if (inst->source->type == Immediate) {
            source_word = get_immediate_operand_word(assembler, inst->source);
        }
        if (inst->source->type == Direct) {
            source_word = get_direct_operand_word(assembler, inst->source);
        }
        if (inst->source->type == Register) {
            source_word = get_register_operand_word(assembler, inst->source);
        }
        if (source_word != NULL) {
            source_word->content[12] = '\0';
            add_word(assembler, source_word);
        }
    }
    Node *dest_word = empty_word();
    if (inst->destination->type == Immediate) {
        dest_word = get_immediate_operand_word(assembler, inst->destination);
    }
    if (inst->destination->type == Direct) {
        dest_word = get_direct_operand_word(assembler, inst->destination);
    }
    if (inst->destination->type == Register) {
        dest_word = get_register_operand_word(assembler, inst->destination);
    }
    if (dest_word != NULL) {
        dest_word->content[12] = '\0';
        add_word(assembler, dest_word);
    }
    return;
}

Node *get_direct_operand_word(Assembler *assembler, Operand *operand) {
    Symbol *symbol = get_symbol_by_name(assembler->symbol_table, operand->value);
    if (symbol == NULL) {
        add_error(assembler, "No matching symbol for symbol %s in line %s", operand->value, assembler->current_line);
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
        printf("Memory allocation failed.\n");   
        exit(1); // todo :: create exit method to clear everything
    }
    word->content = (char *)malloc(13);
    word->next = NULL;
    return word;
}

Node *get_word_node(char data[13]) {
    Node *word = (Node *)malloc(sizeof(Node));
    if (word == NULL) {
        printf("Memory allocation failed.\n");   
        exit(1); // todo :: create exit method to clear everything
    }
    word->content = (char *)malloc(13);
    strncpy(word->content, data, 12);
    word->content[12] = '\0';
    word->next = NULL;
    return word;
}

void add_word(Assembler *assembler, Node *word) {
    add_node_to_list(assembler->words_l_list, word);
    debug("current word is %s", assembler->words_l_list->tail->content);
}

// todo :: improve this code
char *to_binary(unsigned int num, unsigned int length) {
    char *binary = (char *)malloc((length + 1) * sizeof(char));
    if (binary == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    binary[length] = '\0';

    for (int i = length - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return binary;
}
