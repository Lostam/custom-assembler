#include <stdlib.h>
#include <string.h>

#include "token.h"

struct token_t {
	tokenType type;
	char *text;
};

token newToken( char *text, tokenType type ) {
	token tok = malloc( sizeof( *tok ) );

	tok->type = type;
	tok->text = malloc( strlen( text ) + 1 );

	strcpy( tok->text, text );

	return tok;
}

void deleteToken( token tok ) {
	free( tok->text );
	free( tok );
}

tokenType getTokType( token tok ) {
	return tok->type;
}

/* The char* returned points to the token object's internal memory and it
 * might go away, so if you want to do anything with the string copy it, don't
 * store a pointer to it.
 */
char *getTokenString( token tok ) {
	return tok->text;
}
