#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "assembler.h"
#include "symTabFacade.h"
#include "vector.h"
#include "stoa.h"
#include "tokenizer.h"
#include "instruction.h"

struct assembler_t {
	int hasError;
	symTabFacade symTab;
	vector instructions;
	vector data;
	tokenizer toker;
};

static const char asmFileExt[] = ".as";
/* subtract 1 for the null terminating character */
#define ASM_FILE_EXT_LEN ( ( sizeof(asmFileExt) - 1 ) / sizeof(asmFileExt[0]) )
#define CODE_BASE_ADDRESS 100

/* TODO Get rid of any remaining magic numbers */

assembler newAssembler(  ) {
	assembler as = malloc( sizeof( *as ) );

	as->hasError = 0;
	as->symTab = newSymTabFacade(  );
	as->instructions = newVector( 8 );
	as->data = newVector( 8 );
	as->toker = newTokenizer(  );

	return as;
}

void clearAssembler( assembler as ) {
	as->hasError = 0;
	clearSymTabFacade( as->symTab );
	vectorClear( as->instructions, ( deleteFunc ) deleteInstruction );
	vectorClear( as->data, NULL );
}

void deleteAssembler( assembler as ) {
	deleteSymTabFacade( as->symTab );
	deleteVector( as->instructions, ( deleteFunc ) deleteInstruction );
	deleteVector( as->data, NULL );
	deleteTokenizer( as->toker );

	free( as );
}

void asmSetError( assembler as ) {
	as->hasError = 1;
}

int asmHasError( assembler as ) {
	return as->hasError;
}

/* TODO This function is just way too big, need to break it up */
int assemble( assembler as, char *filename ) {
	FILE *ifp;
	unsigned short IC = CODE_BASE_ADDRESS, iIdx = 0, dIdx = 0;
	char line[INPUT_LINE_SIZE + 1];
	char fnameBuf[MAX_FILENAME_LEN + ASM_FILE_EXT_LEN];
	char *fname = fnameBuf;	/* fname will point to the filename without the extension */

	{
		int len = strlen( filename );

		if ( len > MAX_FILENAME_LEN ) {
			fprintf( stderr,
				 "Error: File name exceeds 256 chars.\n" );
			return -1;
		}

		/* check for presence of the file extension */
		if ( strcmp( asmFileExt, filename + ( len - ASM_FILE_EXT_LEN ) )
		     != 0 ) {
			fname = filename;
			sprintf( fnameBuf, "%s%s", fname, asmFileExt );
			filename = fnameBuf;
		} else {
			strncpy( fname, filename, len - ASM_FILE_EXT_LEN );
			fname[len - ASM_FILE_EXT_LEN] = '\0';
		}

	}

	if ( ( ifp = fopen( filename, "r" ) ) == NULL ) {
		perror( "assemble: " );
		fprintf( stderr, "Unable to open %s\n", filename );
		return -1;
	}

	while ( fgets( line, INPUT_LINE_SIZE, ifp ) != NULL ) {
		token tok = NULL, label = NULL;
		tokenType tokType;

		/* printf( "Input Line:\t%s", line ); */

		parseLine( as->toker, line );

		tok = getNextToken( as->toker );
		tokType = getTokType( tok );

		if ( tokType == tokLabel ) {
			label = tok;
			tok = getNextToken( as->toker );
			tokType = getTokType( tok );
		}

		switch ( tokType ) {
		case tokCmd:
			{
				instruction inst =
				    newInstruction( getTokenString( tok ), IC );
				token src;

				deleteToken( tok );
				tok = getNextToken( as->toker );
				tokType = getTokType( tok );
				if ( tokType == tokOp )
					src = tok;
				else if ( tokType == tokNL )
					goto ADDINST;
				else {
					asmSetError( as );
					break;
				}

				tok = getNextToken( as->toker );
				tokType = getTokType( tok );
				if ( tokType == tokOp ) {
					/* got 2 ops, add them both */
					if ( addSrcOp
					     ( inst, getTokenString( src ) )
					     || addDstOp( inst,
							  getTokenString
							  ( tok ) ) ) {
						/* one of the operands types is incorrect for this
						 * instruction
						 */
						fprintf( stderr,
							 "Error: Invalid operand %s or %s\n",
							 getTokenString( src ),
							 getTokenString
							 ( tok ) );

						asmSetError( as );
						break;
					}
				} else {
					/* only got one op so it goes in the Dst slot */
					if ( addDstOp
					     ( inst, getTokenString( src ) ) ) {
						fprintf( stderr,
							 "Error: Invalid operand %s\n",
							 getTokenString
							 ( src ) );

						asmSetError( as );
						break;
					}
				}
				deleteToken( src );
 ADDINST:

				if ( label != NULL ) {
					stfAddLabel( as->symTab,
						     getTokenString( label ),
						     getLabelType( ".code" ),
						     IC );
					deleteToken( label );
					label = NULL;
				}

				vecAppendObject( as->instructions, inst );
				IC += getInstSize( inst );
			}
			break;
		case tokDec:
			{
				/* .data, .string, .extern, .entry */
				labelType decType =
				    getLabelType( getTokenString( tok ) );

				switch ( decType ) {
				case labelData:
					if ( label != NULL ) {
						stfAddLabel( as->symTab,
							     getTokenString
							     ( label ), decType,
							     ( unsigned short )
							     vecGetSize( as->
									 data ) );
						deleteToken( label );
						label = NULL;
					}

					/* get all the integer tokens and add them to the data array
					 */
					while ( 1 ) {
						deleteToken( tok );
						tok = getNextToken( as->toker );

						if ( getTokType( tok ) ==
						     tokNL )
							break;
						else if ( getTokType( tok ) ==
							  tokError ) {
							fprintf( stderr,
								 "Unexpected token %s\n",
								 getTokenString
								 ( tok ) );
							break;
						}

						vecAppendUS( as->data,
							     ( unsigned short )
							     atoi
							     ( getTokenString
							       ( tok ) ) );
					}
					break;
				case labelString:
					if ( label != NULL ) {
						stfAddLabel( as->symTab,
							     getTokenString
							     ( label ), decType,
							     ( unsigned short )
							     vecGetSize( as->
									 data ) );
						deleteToken( label );
						label = NULL;
					}

					/* get the string and add it to the data one char at a time */
					deleteToken( tok );
					tok = getNextToken( as->toker );

					if ( getTokType( tok ) != tokString ) {
						fprintf( stderr,
							 "Unexpected token %s\n",
							 getTokenString
							 ( tok ) );
						break;
					}
					{
						char *s = getTokenString( tok );
						while ( *s )
							vecAppendUS( as->data,
								     ( unsigned
								       short )
								     *s++ );

						/* append the NULL term char */
						vecAppendUS( as->data,
							     ( unsigned short )
							     *s++ );
					}

					break;
				case labelExtern:
				case labelEntry:
					/* Next token must be a valid tokLabel */
					if ( label != NULL ) {
						fprintf( stderr,
							 "Warning: Ignoring redundant extern / entry label: %s\n",
							 getTokenString
							 ( label ) );
						deleteToken( label );
						label = NULL;
					}

					deleteToken( tok );

					tok = getNextToken( as->toker );
					if ( getTokType( tok ) != tokLabel ) {
						asmSetError( as );
						break;
					}
					/* We don't care about redundant extern or entry declarations */
					stfAddLabel( as->symTab,
						     getTokenString( tok ),
						     decType, 0 );
					break;
				default:
					fprintf( stderr,
						 "Invalid declaration type: %s\n",
						 getTokenString( tok ) );
					asmSetError( as );
					break;
				}
			}
			break;
		case tokNL:
			/* Comment or blank line get next line of input */
			break;
		case tokError:
		default:
			asmSetError( as );
			fprintf( stderr,
				 "How did we get here?  The chicken or the egg?\n" );
		}
		deleteToken( tok );
	}			/* while */

	/* Iterate
	 * through the instruction list having each instruction output itself to
	 * the code segment.  The symTab will automagically adjust the returned
	 * data segment offsets adjusted to be offset from the end of the code
	 * segment. 
	 */
	{
		char obFileName[MAX_FILENAME_LEN + 4];
		char outputLine[MAX_OUTPUT_LINE_LEN + 1];
		FILE *obfp;
		int outputIdx = 0;

		stfSetDataSegmentBase( as->symTab, IC );

		for ( iIdx = 0;
		      iIdx != ( unsigned short )vecGetSize( as->instructions );
		      iIdx++ ) {
			instruction inst =
			    vecGetObject( as->instructions, ( int )iIdx );
			if ( resolveInstruction( inst, as->symTab ) )
				asmSetError( as );
		}

		/* Finally output everything to files */
		if ( asmHasError( as ) ) {
			fprintf( stderr, "Compilation of %s failed...\n",
				 filename );
			goto CLEANUP;
		}

		/* Now dump the extern and entry tables. If non-zero returned then
		 * there was an error dumping the entry table due to an unresolved
		 * entry point ( i.e. someone declared ".entry foo" but the code label
		 * foo doesn't exist. */
		if ( stfDump( as->symTab, fname ) ) {
			fprintf( stderr, "Unresolved symbol in entry table\n" );
			goto CLEANUP;
		}

		sprintf( obFileName, "%s.ob", fname );

		if ( ( obfp = fopen( obFileName, "w" ) ) == NULL ) {
			fprintf( stderr, "Failed to open %s\n", obFileName );
			goto CLEANUP;
		}

		/* write the length of the data segment and the code segment in base
		 * 12
		 */
		outputIdx =
		    ustoa( ( unsigned short )IC - CODE_BASE_ADDRESS, 12,
			   outputLine, MAX_OUTPUT_LINE_LEN, 0 );
		outputLine[outputIdx++] = ' ';
		ustoa( ( unsigned short )vecGetSize( as->data ), 12,
		       &outputLine[outputIdx], MAX_OUTPUT_LINE_LEN - outputIdx,
		       0 );
		fprintf( obfp, "%s\n", outputLine );

		/* Write the code segment ( base 12 )
		 * address "instruction word" "link info"
		 */
		for ( iIdx = 0;
		      iIdx != ( unsigned short )vecGetSize( as->instructions );
		      iIdx++ ) {
			instruction ins =
			    vecGetObject( as->instructions, ( int )iIdx );
			printInstruction( ins, outputLine );
			fprintf( obfp, "%s", outputLine );
		}

		/* write out the data segment ( base 12 )
		 * address "data word"
		 */
		for ( dIdx = 0;
		      dIdx != ( unsigned short )vecGetSize( as->data );
		      dIdx++ ) {
			outputIdx =
			    ustoa( IC + dIdx, 12, outputLine,
				   MAX_OUTPUT_LINE_LEN, 4 );
			outputLine[outputIdx++] = ' ';
			ustoa( vecGetUS( as->data, ( int )dIdx ), 12,
			       &outputLine[outputIdx],
			       MAX_OUTPUT_LINE_LEN - outputIdx, 8 );
			fprintf( obfp, "%s\n", outputLine );
		}

		fclose( obfp );
	}

 CLEANUP:

	fclose( ifp );

	clearAssembler( as );

	if ( asmHasError( as ) )
		return -1;

	return 0;
}
