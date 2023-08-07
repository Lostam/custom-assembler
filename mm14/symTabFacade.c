#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "symTabFacade.h"
#include "bst.h"
#include "vector.h"
#include "symbol.h"
#include "stoa.h"

struct symTabFacade_t {
	bstNode symTab;
	vector externTab;
	vector entryTab;
	unsigned short dataSegmentBase;
};

symTabFacade newSymTabFacade(  ) {
	symTabFacade stf = malloc( sizeof( *stf ) );

	stf->dataSegmentBase = 0;
	stf->symTab = NULL;
	stf->externTab = newVector( 4 );
	stf->entryTab = newVector( 4 );

	return stf;
}

void deleteSymTabFacade( symTabFacade stf ) {
	if ( stf->symTab != NULL )
		deleteBst( stf->symTab, ( deleteFunc ) deleteSymbol );
	deleteVector( stf->entryTab, ( deleteFunc ) deleteSymbol );
	deleteVector( stf->externTab, ( deleteFunc ) deleteSymbol );
	free( stf );
}

void clearSymTabFacade( symTabFacade stf ) {
	stf->dataSegmentBase = 0;
	deleteBst( stf->symTab, ( deleteFunc ) deleteSymbol );
	stf->symTab = NULL;
	vectorClear( stf->entryTab, ( deleteFunc ) deleteSymbol );
	vectorClear( stf->externTab, ( deleteFunc ) deleteSymbol );
}

/* Adds label to symTab and checks for dublicate declarations 
 * @returns 0 for success, non-zero on error 
 */
int stfAddLabel( symTabFacade stf, const char *label, labelType labType,
		 unsigned short offset ) {
	if ( labType == labelEntry )
		return vecAppendObject( stf->entryTab,
					newSymbol( label, labType, offset ) );
	else
		return bstInsert( &stf->symTab,
				  newSymbol( label, labType, offset ),
				  ( cmpFunc ) cmpSymbols );
}

/* Get's the address of the requested symbol.  If the symbol requested is
 * extern then it updates the extern table with the address of the operand
 * requesting the lookup.
 */
int stfGetLabelAddress( symTabFacade stf, const char *label,
			unsigned short offset ) {
	symbol sym = newSymbol( label, labelData, 0 );
	bstNode n = bstFind( stf->symTab, sym, ( cmpFunc ) cmpSymbols );

	deleteSymbol( sym );
	if ( n != NULL
	     && strcmp( label, getSymLabel( sym = bstGetData( n ) ) ) == 0 ) {
		labelType type = getSymType( sym );
		if ( type == labelExtern ) {
			vecAppendObject( stf->externTab,
					 newSymbol( label, labelExtern,
						    offset ) );
			return 0;
		} else if ( type == labelData || type == labelString ) {
			return ( int )getSymOffset( sym ) +
			    stf->dataSegmentBase;
		}
		/* code label */
		return ( int )getSymOffset( sym );
	}
	return -1;
}

/* Writes the extern and entry tables to disk and makes sure that any entry
 * symbols actually exist in the symbol table.  If they don't it doesn't dump
 * anything and returns an error.
 */
int stfDump( symTabFacade stf, const char *fname ) {
	int i;
	char outfileName[MAX_FILENAME_LEN + 1];
	FILE *fp;

	for ( i = 0; i != vecGetSize( stf->entryTab ); i++ ) {
		symbol sym = vecGetObject( stf->entryTab, i );
		bstNode n = bstFind( stf->symTab, sym, ( cmpFunc ) cmpSymbols );
		if ( n == NULL
		     || strcmp( getSymLabel( bstGetData( n ) ),
				getSymLabel( sym ) ) != 0 ) {
			fprintf( stderr, "Undefined entry label %s\n",
				 getSymLabel( sym ) );
			return -1;
		}
		setSymOffset( sym,
			      getSymOffset( bstGetData( n ) ) +
			      ( getSymType( bstGetData( n ) ) ==
				labelData ? stf->dataSegmentBase : 0 )
		     );
	}
	/* all the entry labels are defined, lets dump the extern and entry tables
	 */
	if ( vecGetSize( stf->entryTab ) != 0 ) {
		strcpy( outfileName, fname );
		strncat( outfileName, ".ent", MAX_FILENAME_LEN );

		if ( ( fp = fopen( outfileName, "w" ) ) == NULL ) {
			perror( "stfDump" );
			return -1;
		}

		for ( i = 0; i != vecGetSize( stf->entryTab ); i++ ) {
			symbol sym = vecGetObject( stf->entryTab, i );
			char buf[16];

			ustoa( getSymOffset( sym ), 12, buf, 16, 4 );
			fprintf( fp, "%s\t%s\n", getSymLabel( sym ), buf );
		}
		fclose( fp );
	}

	if ( vecGetSize( stf->externTab ) != 0 ) {
		strcpy( outfileName, fname );
		strncat( outfileName, ".ext", MAX_FILENAME_LEN );

		if ( ( fp = fopen( outfileName, "w" ) ) == NULL ) {
			perror( "stfDump" );
			return -1;
		}

		for ( i = 0; i != vecGetSize( stf->externTab ); i++ ) {
			symbol sym = vecGetObject( stf->externTab, i );
			char buf[16];

			ustoa( getSymOffset( sym ), 12, buf, 16, 4 );
			fprintf( fp, "%s\t%s\n", getSymLabel( sym ), buf );
		}
		fclose( fp );
	}
	return 0;
}

/* This sets the base to which data segment addresses are relative to.  This
 * is basicaly a nasty cludge ( like a few things in this code ) to deal with
 * the relative index operand type.
 */
void stfSetDataSegmentBase( symTabFacade stf, unsigned short base ) {
	stf->dataSegmentBase = base;
}
