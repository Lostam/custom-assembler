#include <stdlib.h>
#include <string.h>

#include "symbol.h"

struct symbol_t {
	char *label;
	labelType type;
	unsigned short offset;
};

symbol newSymbol( const char *label, labelType type, unsigned short offset ) {
	symbol sym = malloc( sizeof( *sym ) );

	sym->label = malloc( 31 );
	strncpy( sym->label, label, 30 );
	sym->label[strlen( label )] = '\0';

	sym->type = type;
	sym->offset = offset;

	return sym;
}

void deleteSymbol( symbol sym ) {
	free( sym->label );
	free( sym );
}

static struct {
	const char *decString;
	labelType type;
} decTab[] = {
	{
	".code", labelCode}, {
	".data", labelData}, {
	".extern", labelExtern}, {
	".entry", labelEntry}, {
".string", labelString},};

labelType getLabelType( const char *label ) {
	int i;

	for ( i = 0; i != ( sizeof( decTab ) / sizeof( decTab[0] ) ); i++ ) {
		if ( strcmp( decTab[i].decString, label ) == 0 )
			return decTab[i].type;
	}
	return labelError;
}

char *getSymLabel( symbol sym ) {
	return sym->label;
}

labelType getSymType( symbol sym ) {
	return sym->type;
}

void setSymOffset( symbol sym, unsigned short offset ) {
	sym->offset = offset;
}

unsigned short getSymOffset( symbol sym ) {
	return sym->offset;
}

int cmpSymbols( symbol l, symbol r ) {
	return strcmp( l->label, r->label );
}
