#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "instruction.h"

int tests_run = 0;

/*@null@*/
static const char *test_newInstruction_valid_instruction(  ) {
    instruction inst = newInstruction( "mov", 0xbeef );

    mu_assert("newInstruction failed unexpectedly" ,  inst != NULL );

    deleteInstruction( inst );

	return 0;
}

/*@null@*/
static const char *test_newInstruction_invalid_instruction(  ) {
    instruction inst = newInstruction( "foo", 0xbeef );

    mu_assert("newInstruction accepted invalid command" ,  inst == NULL );

	return 0;
}

/*@null@*/
static const char *test_mov_addSrcOp_addDstOp(  ) {
    char *ops[] = {
        "#-123",
        "foo",
        "foo[*bar]",
        "foo[bar][r1]",
        "r2",
        };
    int i;

    for ( i = 0; i != (sizeof(ops)/sizeof(ops[0])); i++ ) {
        instruction inst = newInstruction( "mov", 0x1 );

        printf("Adding src op %s\n", ops[i]);

        mu_assert( "addSrcOp failed", addSrcOp( inst, ops[i] ) == 0 );

        deleteInstruction( inst );
    }

    for ( i = 1; i != (sizeof(ops)/sizeof(ops[0])); i++ ) {
        instruction inst = newInstruction( "mov", 0x1 );

        printf("Adding src op %s\n", ops[i]);

        mu_assert( "addSrcOp failed", addDstOp( inst, ops[i] ) == 0 );

        deleteInstruction( inst );
    }

	return 0;
}

/*@null@*/
static const char *test_lea_addSrcOp_addDstOp_invalid_opType(  ) {
    char *ops[] = {
        "#-123",
        "r2",
        };

    instruction inst = newInstruction( "lea", 0x1 );

    mu_assert("Accepted Invalid op type", addSrcOp( inst, ops[0] ) != 0 );
    mu_assert("Accepted Invalid op type", addSrcOp( inst, ops[1] ) != 0 );
    mu_assert("Accepted Invalid op type", addDstOp( inst, ops[0] ) != 0 );

    deleteInstruction( inst );

	return 0;
}

/*@null@*/
static const char *test_getInst(  ) {
    char *ops[] = {
        "#-123",
        "foo[bar][r2]",
        };

    instruction inst = newInstruction( "mov", 0x1 );

    mu_assert("Accepted Invalid op type", addSrcOp( inst, ops[0] ) == 0 );
    mu_assert("Accepted Invalid op type", addDstOp( inst, ops[1] ) == 0 );

    mu_assert("Returned invalid instruction size", getInstSize( inst ) == 4 );

    deleteInstruction( inst );

	return 0;
}

/*@null@*/
static const char *test_inst_resolveInst_printInst(  ) {
    /* the instruction in binary: 0000 0000 0001 1010 */
    char *result = "0001 00000022 A\n0002 00031B13 A\n0003 0000000A R\n0004 00000009 R\n";
    symTabFacade stf = NULL;
    char buf[81];

    instruction inst = newInstruction( "mov", 0x1 );

    mu_assert("Accepted Invalid op type", addSrcOp( inst, "#-1" ) == 0 );
    mu_assert("Accepted Invalid op type", addDstOp( inst, "foo[bar][r2]" ) == 0 );

    mu_assert("resolveInstruction failed", resolveInstruction( inst, stf ) == 0 );
    mu_assert("printInstruction failed", printInstruction( inst, buf ) == 0 );
    printf("%s", buf);
    mu_assert("printInstruction output doesn't match", strcmp(buf, result) == 0);

    deleteInstruction( inst );

    /* 0100 000 000 011 010 */
    result = "0001 000095B6 A\n0002 0000000A R\n0003 00000009 R\n";
    inst = newInstruction( "not", 0x1 );

    mu_assert("Accepted Invalid op type", addDstOp( inst, "foo[bar][r2]" ) == 0 );

    mu_assert("resolveInstruction failed", resolveInstruction( inst, stf ) == 0 );
    mu_assert("printInstruction failed", printInstruction( inst, buf ) == 0 );
    printf("%s", buf);
    mu_assert("printInstruction output doesn't match", strcmp(buf, result) == 0);

    deleteInstruction( inst );

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_newInstruction_valid_instruction );
	mu_run_test( test_newInstruction_invalid_instruction );
    mu_run_test( test_mov_addSrcOp_addDstOp );
    mu_run_test( test_lea_addSrcOp_addDstOp_invalid_opType );
    mu_run_test( test_getInst );
    mu_run_test( test_inst_resolveInst_printInst );

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
