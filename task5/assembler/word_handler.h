#ifndef WORD_HANDLER_H
#define WORD_HANDLER_H

#include "statement_handler.h"
#include "instruction_handler.h"
#include "directive_handler.h"
#include "utils.h"
#include "operation_handler.h"

int count_required_words(Statement *);
void add_words_to_assembler(Assembler *, Statement *);
void handle_data_directive(Assembler *, Directive *);
void handle_string_directive(Assembler *, Directive *);
void handle_instruction(Assembler *, Instruction *);
void handle_operation(Assembler *, Instruction *);
void handle_operands(Assembler *, Instruction *);
Node *get_register_operand_word(Assembler *, Operand *);
Node *get_immediate_operand_word(Assembler *, Operand *);
Node *get_direct_operand_word(Assembler *, Operand *);
Node *get_word_node(char [13]);
Node *empty_word();
void add_word(Assembler *, Node *);
char *to_binary(unsigned int, unsigned int);
#endif /* WORD_HANDLER_H */