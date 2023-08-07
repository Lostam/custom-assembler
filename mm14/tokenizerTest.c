#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "tokenizer.h"

int tests_run = 0;
static tokenizer toker;
static token tok;

/*@null@*/
static const char *test_blankline(  ) {
    char *line = "\t   \t\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_comment(  ) {
    char *line = "; Hello There!\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_invalid_comment(  ) {
    char *line = " ; Hello There!\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokError );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_instruction_no_label_one_arg(  ) {
    char *line[] = { 
        "mov  foo\n",
        "mov r2 \n",
        "mov y[i][r1] \n",
        "mov  y [ i ] [    r0  ] \n",
        "mov y[*j] \n",
        "mov  y  [ *   j  ]  \n",
        "mov #123 \n",
        "mov #-123\n",
        
        };
    int i ;

    for ( i = 0; i != ( sizeof( line ) / sizeof(line[0]) ); i++ ) { 
        printf("Parseing %s", line[i]);
        parseLine(toker, line[i]);

        tok = getNextToken(toker);

        mu_assert("Got wrong token type" , getTokType(tok) == tokCmd );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokOp );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokNL );

        deleteToken(tok);
    }
	return 0;
}

/*@null@*/
static const char *test_instruction_no_label_two_args(  ) {
    char *line[] = { 
        "mov  foo  ,  r1\n",
        "mov foo,r1\n",
        "mov r2,r1\n",
        "mov y[i][r1],r1\n",
        "mov  y [ i ] [    r0  ]  ,  r1 \n",
        "mov  r1, y [i] [    r7  ]  \n",
        "mov y[*j],r1\n",
        "mov  r1, y  [ *   j  ]  \n",
        "mov #123,r1\n",
        "mov #-123,r1\n",
        "mov   r1  ,   #+123\n",
        "mov #0,r1\n",
        
        };
    int i ;

    for ( i = 0; i != ( sizeof( line ) / sizeof(line[0]) ); i++ ) { 
        printf("Parseing %s", line[i]);
        parseLine(toker, line[i]);

        tok = getNextToken(toker);

        mu_assert("Got wrong token type" , getTokType(tok) == tokCmd );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokOp );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokOp );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokNL );

        deleteToken(tok);
    }
	return 0;
}

/*@null@*/
static const char *test_instruction_with_label_two_args(  ) {
    char *line = "codeLabel1: mov r[*j1],r1\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokLabel );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokCmd );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokOp );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokOp );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_instruction_with_label_zero_args(  ) {
    char *line = "codeLabel1: hlt \n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokLabel );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokCmd );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_instruction_with_invalid_label_zero_args(  ) {
    char *line[] = { 
        "r1: hlt \n",
        "mov: hlt \n",
        };
    int i;
    
    for ( i = 0; i != ( sizeof(line) / sizeof(line[0])); i++ ) {
        parseLine(toker, line[i]);

        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokError );

        deleteToken(tok);
    }
	return 0;
}

/*@null@*/
static const char *test_invalid_instruction_with_label_missing_space(  ) {
    char *line = "codeLabel1: movr[*j1],r1\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokLabel );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokError );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_instruction_no_label_invalid_first_arguement(  ) {
    char *line[] = { 
        "mov  1foo  ,  r1\n", /* invalid label */
        "mov  foo123456789012345678901234567890  ,  r1\n",/* label too long */
        "mov foo*,r1\n",/* invalid char following label */
        "mov y[i],r1\n",/* missing second index */
        "mov  y [ i ] [    *r0  ]  ,  r1 \n",/* invalid second index */
        "mov  r1[, y [i] [    7  ]  \n",/* unbalanced [ */
        "mov  r1[a , y [i] [    7  ]  \n",/* unbalanced [ */
        "mov #123U,r1\n",/* invalid literal */
        "mov # -123,r1\n",/* invalid literal */
        
        };
    int i ;

    for ( i = 0; i != ( sizeof( line ) / sizeof(line[0]) ); i++ ) { 
        printf("Parseing %s", line[i]);
        parseLine(toker, line[i]);

        tok = getNextToken(toker);

        mu_assert("Got wrong token type" , getTokType(tok) == tokCmd );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokError );

        deleteToken(tok);
    }
	return 0;
}

/*@null@*/
static const char *test_invalid_label_not_in_first_col(  ) {
    char *line = " codeLabel1: movr[*j1],r1\n";

    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokError );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_extern_entry_no_start_label(  ) {
    char *line[] = {
        "  .extern   foo\n",
        ".entry foo \n",
        };
    int i;

    for ( i = 0; i != 2; i++) {
        printf("Parseing %s", line[i]);
        parseLine(toker, line[i]);

        tok = getNextToken(toker);

        mu_assert("Got wrong token type" , getTokType(tok) == tokDec );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokLabel );

        deleteToken(tok);
        tok = getNextToken(toker);

        mu_assert("Got wrong token type", getTokType(tok) == tokNL );

        deleteToken(tok);
    }
	return 0;
}


/*@null@*/
static const char *test_data_no_start_label(  ) {
    char *line = ".data +123,  -456, 789 \n";

    printf("Parseing %s", line);
    parseLine(toker, line);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokDec );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNum );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNum );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNum );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_string_no_start_label(  ) {
    char *line[] = {
        ".string \"dsf 123 \\\"'!@#$%^&*()+_=-\" \n",
        };
    int i = 0;

    printf("Parseing %s", line[i]);
    parseLine(toker, line[i]);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokDec );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokString );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *test_string_with_start_label(  ) {
    char *line[] = {
        "trash: .string \"dsf 123 \\\"'!@#$%^&*()+_=-\" \n",
        };
    int i = 0;

    printf("Parseing %s", line[i]);
    parseLine(toker, line[i]);

    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokLabel );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type" , getTokType(tok) == tokDec );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokString );

    deleteToken(tok);
    tok = getNextToken(toker);

    mu_assert("Got wrong token type", getTokType(tok) == tokNL );

    deleteToken(tok);

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_blankline );
	mu_run_test( test_comment );
	mu_run_test( test_invalid_comment );
	mu_run_test( test_instruction_no_label_two_args );
	mu_run_test( test_instruction_no_label_one_arg );
	mu_run_test( test_instruction_with_label_two_args );
	mu_run_test( test_instruction_with_label_zero_args );
	mu_run_test( test_instruction_with_invalid_label_zero_args );
    mu_run_test( test_invalid_instruction_with_label_missing_space );
    mu_run_test( test_instruction_no_label_invalid_first_arguement );
    mu_run_test( test_invalid_label_not_in_first_col );
    mu_run_test( test_extern_entry_no_start_label );
    mu_run_test( test_data_no_start_label );
    mu_run_test( test_string_no_start_label );
    mu_run_test( test_string_with_start_label );

	return 0;
}

int main( void ) {
    const char *result;
    toker = newTokenizer();

	result = all_tests(  );
	if ( result != 0 ) {
		printf( "%s\n", result );
	} else {
		printf( "ALL TESTS PASSED\n" );
	}
	printf( "Tests run: %d\n", tests_run );

    deleteTokenizer(toker);

	return ( int )( result != 0 );
}
