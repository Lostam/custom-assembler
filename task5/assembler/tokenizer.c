#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "tokenizer.h"
#include "defs.h"

struct tokeninzer_t {
	char *line;
	int lineIdx, lineLen;
	tokenType lastTokType;
};

tokenizer newTokenizer( void ) {
	tokenizer toker = malloc( sizeof( *toker ) );
	toker->line = NULL;
	toker->lineIdx = -1;
	toker->lineLen = 0;

	return toker;
}

void parseLine( tokenizer toker, char *line ) {
    int len = strlen( line );

	if ( len > toker->lineLen ) {
        if ( ( toker->line = realloc( toker->line, len + 1 ) ) == NULL ) {
            perror( "parseLine: " );
            exit( EXIT_FAILURE );
        }
    }
	strcpy( toker->line, line );

	toker->lineIdx = 0;
	toker->lastTokType = tokNL;
    toker->lineLen = len;
}

/* @returns the number of leading whitespace's */
int skipWhiteSpace( char *s ) {
	int i = 0;
	while ( isspace( s[i] ) )
		i++;
	return i;
}

/* checks if a valid variable name occurs at the beginning of the string
 * %1[a-zA-Z]%29[a-zA-Z0-9]
 * followed by whitespace or an operator:  , [ ] :
 * @returns the length of the variable name, if present, 0 if there isn't a
 * valid variable name, -1 on error.
 */
int getVarName( const char *s ) {
	int i = 0;

	if ( isalpha( s[i++] ) ) {
		while ( isalnum( s[i] ) && i != 30 )
			i++;
		if ( isspace( s[i] )
		     || s[i] == ','
		     || s[i] == '[' || s[i] == ']' || s[i] == ':' )
			return i;
		else {
			return -1;
		}
	}

	return 0;
}

int getNumber( const char *s ) {
	int i = 0;

	if ( s[i] == '-' || s[i] == '+' )
		i++;

	while ( isdigit( s[i] ) )
		i++;

	if ( !isspace( s[i] ) && s[i] != ',' )
		return -1;

	return i;
}

int illegalLabel( const char *s ) {
	int len = strlen( s );

	if ( len == 2 && s[0] == 'r' && isdigit( s[1] ) ) {
		return 1;
	}

	/* TODO This should be the responsibility of a funtion exported by the
	 * instruction class */
	if ( len == 3 ) {
		char *keywords[] = {
			"mov",
			"cmp",
			"add",
			"sub",
			"not",
			"clr",
			"lea",
			"inc",
			"dec",
			"jmp",
			"bne",
			"red",
			"prn",
			"jsr",
			"rts",
			"hlt",
		};
		int i;
		for ( i = 0;
		      i != ( sizeof( keywords ) / sizeof( keywords[0] ) ); i++ )
			if ( strcmp( keywords[i], s ) == 0 )
				return 1;
	}

	return 0;
}

/* TODO Make all internal functions static */
/* TODO this function is too long, should break it up into subroutines */
token getNextToken( tokenizer toker ) {
	char *s = &toker->line[toker->lineIdx];
	int i = 0, spaceCount = 0, textIdx = 0;
	char text[INPUT_LINE_SIZE + 1];

	text[0] = '\0';

	/* check for a label declaration.  it must be the first char on the line */
	if ( toker->lineIdx == 0 && isalpha( toker->line[0] ) ) {
		/* this could be a label or a command */
		i = getVarName( s );
		spaceCount = skipWhiteSpace( s );
		/* if this is a label the next char must be : */
		if ( s[i + spaceCount] == ':' ) {
			strncpy( text, s, i );
			text[i] = '\0';

			if ( illegalLabel( text ) ) {
				fprintf( stderr, "Illegal Label: %s\n", text );
				return newToken( "", tokError );
			}

			toker->lineIdx += i + spaceCount + 1;
			toker->lastTokType = tokLabel;

			return newToken( text, tokLabel );
		} else
			i = 0;
	}

	/* check for a comment line */
	if ( toker->lineIdx == 0 && toker->line[0] == ';' )
		return newToken( "", tokNL );

	/* check for a blank line or end of line, and eliminate any leading white space */
	spaceCount = skipWhiteSpace( s );

	if ( toker->lineIdx + spaceCount == toker->lineLen ) {
		return newToken( "", tokNL );
	}

	i += spaceCount;

	/* check for a command */
	if ( ( toker->lastTokType == tokNL || toker->lastTokType == tokLabel )
	     && sscanf( &s[i], "%3[a-z]", text ) == 1 ) {

		if ( !isspace( s[i + 3] ) ) {
			fprintf( stderr,
				 "Missing whitespace after command at %s\n",
				 &s[i] );
			return newToken( "", tokError );
		}

		toker->lineIdx += i + 3;
		toker->lastTokType = tokCmd;

		return newToken( text, tokCmd );
	}

	/* check for an operand */
	if ( toker->lastTokType == tokCmd || toker->lastTokType == tokOp ) {

		int varLen1, varLen2;

		if ( toker->lastTokType == tokOp ) {
			/* the next char must be a comma */
			if ( s[i++] == ',' ) {
				i += skipWhiteSpace( &s[i] );
			} else {
				fprintf( stderr,
					 "Syntax Error, expecting comma\n" );
				return newToken( "", tokError );
			}
		}

		/* check for a literal number */
		if ( s[i] == '#' ) {
			int numLen = getNumber( &s[++i] );

			if ( numLen <= 0 ) {
				fprintf( stderr,
					 "Syntax Error, expecting comma\n" );
				return newToken( "", tokError );
			}
			text[textIdx++] = '#';
			strncpy( &text[textIdx], &s[i], numLen );
			i += numLen;
			textIdx += numLen;

			goto OPRET;
		}

		varLen1 = getVarName( &s[i] );
		if ( varLen1 <= 0 ) {
			fprintf( stderr, "Invalid operand %d at %s\n", varLen1,
				 &s[i] );
			return newToken( "", tokError );
		}
		strncpy( text, &s[i], varLen1 );

		textIdx += varLen1;
		i += varLen1;
		i += skipWhiteSpace( &s[i] );

		if ( s[i] == '[' ) {
			int hasRelIdx = 2;
			while ( hasRelIdx != 0 ) {
				hasRelIdx--;
				text[textIdx++] = s[i++];
				i += skipWhiteSpace( &s[i] );
				if ( hasRelIdx && s[i] == '*' ) {
					hasRelIdx--;
					text[textIdx++] = s[i++];
					i += skipWhiteSpace( &s[i] );
				}
				varLen2 = getVarName( &s[i] );
				if ( varLen2 <= 0 ) {
					fprintf( stderr,
						 "Invalid operand %d at %s\n",
						 varLen2, &s[i] );
					return newToken( "", tokError );
				}
				strncpy( &text[textIdx], &s[i], varLen2 );

				textIdx += varLen2;
				i += varLen2;

				i += skipWhiteSpace( &s[i] );
				if ( s[i] != ']' ) {
					fprintf( stderr,
						 "Missing closing ]\n" );
					return newToken( "", tokError );
				}
				text[textIdx++] = s[i++];

				i += skipWhiteSpace( &s[i] );
				if ( hasRelIdx && s[i] != '[' ) {
					fprintf( stderr,
						 "Expecting '[' at %s\n",
						 &s[i] );
					return newToken( "", tokError );
				}
			}

		}

 OPRET:
		text[textIdx] = '\0';
		toker->lineIdx += i;
		toker->lastTokType = tokOp;

		return newToken( text, tokOp );
	}

	/* check for a declaration */
	if ( ( toker->lastTokType == tokNL || toker->lastTokType == tokLabel )
	     && s[i] == '.' ) {
		int decLen = getVarName( &s[i + 1] );

		if ( decLen <= 0 ) {
			fprintf( stderr, "Invalid declaration type at %s\n",
				 &s[i] );
			return newToken( "", tokError );
		}

		text[textIdx++] = s[i++];
		strncpy( &text[textIdx], &s[i], decLen );
		text[textIdx + decLen] = '\0';

		if ( strcmp( text, ".extern" ) != 0
		     && strcmp( text, ".entry" ) != 0
		     && strcmp( text, ".data" ) != 0
		     && strcmp( text, ".string" ) != 0 ) {
			fprintf( stderr, "Invalid declaration type at %s\n",
				 &s[i] );
			return newToken( "", tokError );
		}

		toker->lineIdx += i + decLen;
		toker->lastTokType = tokDec;

		return newToken( text, tokDec );
	}

	/* check for a target label */
	if ( toker->lastTokType == tokDec ) {
		int varLen = getVarName( &s[i] );

		if ( varLen > 0 ) {
			strncpy( text, &s[i], varLen );
			text[varLen] = '\0';

			if ( illegalLabel( text ) ) {
				fprintf( stderr, "Illegal Label: %s\n", text );
				return newToken( "", tokError );
			}

			toker->lineIdx += i + varLen;
			toker->lastTokType = tokLabel;

			return newToken( text, tokLabel );
		}
	}

	/* check for a number */
	if ( toker->lastTokType == tokDec || toker->lastTokType == tokNum ) {

		if ( toker->lastTokType == tokNum ) {
			if ( s[i] != ',' ) {
				fprintf( stderr,
					 "Syntax Error, expecting comma at %s\n",
					 &s[i] );
				return newToken( "", tokError );
			}
			i++;
			i += skipWhiteSpace( &s[i] );
		}
		textIdx = getNumber( &s[i] );

		if ( textIdx > 0 ) {
			strncpy( text, &s[i], textIdx );
			text[textIdx] = '\0';

			toker->lineIdx += i + textIdx;
			toker->lastTokType = tokNum;

			return newToken( text, tokNum );
		}
	}

	/* check for a string */
	if ( toker->lastTokType == tokDec && s[i] == '"' ) {
		while ( 1 ) {
			text[textIdx++] = s[i++];
			if ( s[i] == '"' && s[i - 1] != '\\' )
				break;
			else if ( s[i] == '\n' ) {
				fprintf( stderr,
					 "Syntax Error, missing closing \"\n" );
				return newToken( "", tokError );
			}
		}
		i++;
		text[textIdx] = '\0';

		toker->lineIdx += i;
		toker->lastTokType = tokString;

		return newToken( text, tokString );
	}

	return newToken( "", tokError );

}

void deleteTokenizer( tokenizer toker ) {
	if ( toker->line != NULL ) {
		free( toker->line );
	}
	free( toker );
}
