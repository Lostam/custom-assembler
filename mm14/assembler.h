#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

typedef struct assembler_t *assembler;

assembler newAssembler();

/* This is the core of the program.  Call it to assemble a source file.  It's
 * ok to call it multiple times using the same assembler object, as it resets
 * itself after each execution.
 *
 * @param as The assembler object to work with.
 * @param filename A filename of an assembly file, with or without the .as
 * file extention.
 *
 * @returns 0 on success, non-zero on failure.
 */
int assemble( assembler as, char* filename );

void deleteAssembler( assembler as );

#endif
