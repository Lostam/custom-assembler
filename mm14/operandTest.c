#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "operand.h"

int tests_run = 0;

/*@null@*/
static const char *test_newOp(  ) {
    char *ops[] = {
        "r2",
        "foo",
        "#-123",
        "foo[bar][r1]",
        "foo[*bar]",
        };
    int i;

    for ( i = 0; i != (sizeof(ops)/sizeof(ops[0])); i++ ) {
        operand op;
        printf("Testing: %s\n", ops[i]);
        op = newOp(ops[i]);

        mu_assert("Error: failed to create operand object" , op != NULL );

        deleteOp(op);
    }

	return 0;
}

/*@null@*/
static const char *test_resolveOp_printOp(  ) {
    char *ops[] = {
        "foo", /* foo 0xA */
        "#-1", 
        "foo[baz][r1]",/* baz 0x0 */
        "foo[*bar]", /* bar 0x9 */
        "foo[*bob]", /* bob 0x2 */
        };
    /* the offsets for the above symbols are taken from the stf Mock */
    const char* results[] = {
        "0006 0000000A R\n",
        "0006 00031B13 A\n",
        "0006 0000000A R\n0007 00000000 E\n",
        "0006 0000000A R\n0007 00000006 A\n",
        "0006 0000000A R\n0007 00031B13 A\n",
    };
    int i;

    symTabFacade stf = NULL;

    for ( i = 0; i != (sizeof(ops)/sizeof(ops[0])); i++ ) {
        operand op;
        opType type;
        char buf[33];
        const unsigned short instOffset = 0x3, opOffset = 0x3;

        printf("Testing: %s\n", ops[i]);
        op = newOp(ops[i]);

        mu_assert("Error: failed to create operand object" , op != NULL );

        type = getOpType( op );
        if ( type == OpDirect || type == Op2D || type == OpRelative )
            mu_assert("resolveOp failed", resolveOp( op, stf, instOffset, opOffset ) == 0 );

        mu_assert("printOp returned 0", printOp(op, buf, instOffset + opOffset ) != 0);

        printf("%s",buf);
        mu_assert("printOp: result doesn't match expected result", strcmp(buf, results[i]) == 0);

        deleteOp(op);
    }

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_newOp );
    mu_run_test( test_resolveOp_printOp );

	return 0;
}

int main( void ) {
    const char *result;

	result = all_tests(  );
	if ( result != 0 ) {
		printf( "%s\n", result );
	} else {
		printf( "ALL TESTS PASSED\n" );
	}
	printf( "Tests run: %d\n", tests_run );

	return ( int )( result != 0 );
}
