#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "symTabFacade.h"

typedef struct operand_t *operand;

typedef enum { OpLiteral = 0, OpDirect, OpRelative, Op2D, OpRegister, OpError = -1 } opType ;

/* Create a new operand object.
 *
 * @param opString A char buffer containing a validated and formated operand
 * string as returned from the tokenizer.
 *
 * @returns A new object on success, or NULL on error.
 */
operand newOp( const char *opString );
void deleteOp( operand op );

opType getOpType( operand op );

/* This can be called either when the opType is Register or 2D. 
 */
int getOpRegNum( operand op );

/* This is called for op types Direct, Relative, and 2D. 
 *
 * @param op The operand object.
 * @param stf The symbol table to perform the lookup in.
 * @param instOffset The Code Segment offset of the containing instruction.
 * @param opOffset The offset of the operand in the code segment relative to
 * the containing instruction.
 *
 * @returns 0 on success, non-zero on failure to resolve an operand.
 * */
int resolveOp( operand op, symTabFacade stf, unsigned short instOffset, unsigned short opOffset );

/* 0 for register, 1 for literal and direct, 2 for relative or 2D */
int getOpSize( operand op );

/* Print the operand in base 12 to the buffer 
 * buf should be large enough to hold 2 printed label offsets plus the
 * relocation character A, E or R 
 * returns a pointer to the buffer including newlines. That about 33 bytes. 
 *
 * @param offset The absolute offset of the operand in the Code Segment.
 */
int printOp( operand op, char *buf, unsigned short offset);

#endif
