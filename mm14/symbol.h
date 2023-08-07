#ifndef _SYMBOL_H_
#define _SYMBOL_H_

typedef struct symbol_t *symbol;

typedef enum { labelError, labelCode, labelData, labelString, labelExtern, labelEntry } labelType;

/* This is a simple storage class with setters and getters and no special
 * responsibilites.
 */
symbol newSymbol( const char* label, labelType type, unsigned short offset );
void deleteSymbol( symbol sym );

/* Translates a label string to a labelType
 *
 * .data .string .extern .entry .code
 */
labelType getLabelType( const char* label );

char *getSymLabel( symbol sym );
labelType getSymType( symbol sym );
void setSymOffset( symbol sym, unsigned short offset );
unsigned short getSymOffset( symbol sym );

/* A comparison function which compares two symbols based on their labels.
 * It's symantics are the same as strcmp.
 */
int cmpSymbols( symbol l, symbol r );
#endif
