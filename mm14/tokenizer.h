#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "token.h"

typedef struct tokeninzer_t *tokenizer;

tokenizer newTokenizer( void );

/* The tokenizer works on one full line at a time.
 *
 * Each call to this function resets the tokenizer
 */
void parseLine( tokenizer toker, char* line );

/* Returns the next token in the current input line, or an error token */
token getNextToken( tokenizer );

void deleteTokenizer( tokenizer );


#endif
