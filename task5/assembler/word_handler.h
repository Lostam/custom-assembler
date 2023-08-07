#ifndef WORD_HANDLER_H
#define WORD_HANDLER_H

#include "statement_handler.h"
#include "instruction_handler.h"
#include "directive_handler.h"
#include "utils.h"
#include "operation_handler.h"

/**
 * @brief Calculate required words for a statement after it has been validated but won't actually create the words
 * should be only called once statement was validated
 */
int count_required_words(Statement *);
/**
 * @brief Add words to assembler based on the type of the statement
 * should be only called once statement was validated
 */
void add_words_to_assembler(Assembler *, Statement *);
/**
 * @brief Adds directive words to the assembler
 */
void handle_data_directive(Assembler *, Directive *);
/**
 * @brief Adds string words to the assembler
 */
void handle_string_directive(Assembler *, Directive *);
/**
 * @brief Adds instruction words to the assembler
 */
void handle_instruction(Assembler *, Instruction *);
/**
 * @brief Adds operation words to the assembler
 */
void handle_operation(Assembler *, Instruction *);
/**
 * @brief Adds operands words to the assembler
 */
void handle_operands(Assembler *, Instruction *);
/**
 * @brief utillity to handle both source and destination operands
 */
void add_operand_word(Assembler *, Operand *);

Node *get_register_operand_word(Assembler *, Operand *);
Node *get_immediate_operand_word(Assembler *, Operand *);
Node *get_direct_operand_word(Assembler *, Operand *);
Node *get_word_node(char [13]);
Node *empty_word();
void add_word(Assembler *, Node *);
/**
 * @brief Converts a char/integer to binary
 */
char *to_binary(unsigned int, unsigned int);
#endif /* WORD_HANDLER_H */