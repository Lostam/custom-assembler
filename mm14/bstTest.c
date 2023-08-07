#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "bst.h"
#include "symbol.h"

int tests_run = 0;

/*@null@*/
static const char *test_bst(  ) {
    bstNode root = NULL;
    struct {
        const char* label;
        labelType type;
        unsigned short offset;
    } syms[] = {
        { "foo", labelData, 0xffff },
        { "bar", labelData, 0xffff },
        { "dead", labelData, 0xffff },
        { "baz", labelData, 0xffff },
        { "beef", labelData, 0xffff },
    };
    int i;
    symbol sym;

    for ( i = 0; i != (sizeof(syms)/sizeof(syms[0])); i++) {
        mu_assert("bstInsert failed",
            bstInsert( &root, newSymbol(syms[i].label, syms[i].type, syms[i].offset), (cmpFunc)cmpSymbols)
                == 0);
    }

    sym = newSymbol(syms[0].label, syms[0].type, syms[0].offset);
    mu_assert("bstInsert accepted duplicate",
        bstInsert( &root, sym, (cmpFunc)cmpSymbols)
            != 0);
    deleteSymbol(sym);

    deleteBst( root, (deleteFunc)deleteSymbol );

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_bst );

	return 0;
}

int main( void ) {
	const char *result = all_tests(  );
	if ( result != 0 ) {
		printf( "%s\n", result );
	} else {
		printf( "ALL TESTS PASSED\n" );
	}
	printf( "Tests run: %d\n", tests_run );

	return ( int )( result != 0 );
}
