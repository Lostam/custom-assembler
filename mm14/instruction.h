#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "symTabFacade.h"

typedef struct instruction_t *instruction;

/* Creata a new instruction object.
 *
 * @param cmd A string representing the assembly command
 * @param IC The offset of the instruction in the Code Segment
 *
 * @returns A new object on success, or NULL when passed an invalid cmd
 * string.
 */
instruction newInstruction( const char* cmd, unsigned short IC );
void deleteInstruction( instruction inst );

/* Adds operand to instruction and checks validity of operand type of the
 * instruction.
 * @returns 0 on success, non-zero on error.
 */
int addSrcOp( instruction inst, const char *op );
int addDstOp( instruction inst, const char *op );

/* calculates how many machine words the instruction and it's operand take up 
 */
int getInstSize( instruction inst );

/* Causes the instruction to lookup and resolve it's operands via the symTab.
 * Should only be called after the whole file has been read and all symbols
 * added to the symbol table.
 * @returns 0 on success, non-zero on non-existant symbol or other error.
 */
int resolveInstruction( instruction inst, symTabFacade stf );

/* Outputs the instruction and it's operands in base 12 format to the given
 * buffer. Should only be called after calling resolve.
 *
 * @param outputBuffer Must be at least 81 bytes.
 * @returns 0 on success, non-zero on error.
 */
int printInstruction( instruction inst, char *outputBuffer );

#endif
