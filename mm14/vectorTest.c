#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "vector.h"
#include "symbol.h"

int tests_run = 0;

/*@null@*/
static const char *test_vector(  ) {
    vector vec = newVector( 1 );
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

    for ( i = 0; i != (sizeof(syms)/sizeof(syms[0])); i++) {
        mu_assert("vecAppendObject failed",
            vecAppendObject( vec, newSymbol(syms[i].label, syms[i].type, syms[i].offset))
                == 0);
    }

    for ( i = 0; i != vecGetSize( vec ); i++ )
        mu_assert("vecGetObject returned unexpected object",
            strcmp(syms[i].label, getSymLabel( vecGetObject( vec, i ) ) ) == 0 );

    vectorClear( vec, (deleteFunc)deleteSymbol );

    for ( i = 0; i != 10; i++ )
        mu_assert("vecAppendUS failed", vecAppendUS( vec, (unsigned short)i) == 0);

    deleteVector( vec, NULL );

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_vector );

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
