#ifndef _SYMTABFACADE_H_
#define _SYMTABFACADE_H_

#include "symbol.h"

typedef struct symTabFacade_t *symTabFacade;

symTabFacade newSymTabFacade( void );
void deleteSymTabFacade( symTabFacade stf );
void clearSymTabFacade( symTabFacade stf );

/* Adds label to symTab and checks for dublicate declarations 
 * @returns 0 for success, non-zero on error 
 */
int stfAddLabel( symTabFacade stf, const char* label, labelType labType, unsigned short offset );

/* Get's the address of the requested symbol.  If the symbol requested is
 * extern then it updates the extern table with the address of the operand
 * requesting the lookup.
 */
int stfGetLabelAddress( symTabFacade stf, const char *label, unsigned short offset );

/* Writes the extern and entry tables to disk and makes sure that any entry
 * symbols actually exist in the symbol table.  If they don't it doesn't dump
 * anything and returns an error.
 */
int stfDump( symTabFacade stf, const char *fname );

/* This sets the base to which data segment addresses are relative to.  This
 * is basicaly a nasty cludge ( like a few things in this code ) to deal with
 * the relative index operand type.
 */
void stfSetDataSegmentBase( symTabFacade stf, unsigned short );

#endif
