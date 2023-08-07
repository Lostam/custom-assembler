#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "stoa.h"

int tests_run = 0;

/*@null@*/
static const char *test_ustoa(  ) {
    unsigned short num = 0;
    char *result;
    char output[16];

    /* check invalid bases */
    mu_assert("error: accepted invalid base", ustoa(num, 1, output, 16, 0) < 0);
    mu_assert("error: accepted invalid base", ustoa(num, 37, output, 16, 0) < 0);

    num = 1;
    result = "1";
    ustoa(num, 16, output, 16, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    /* check for buffer overflow */
    num = 0xffff;
	mu_assert( "error, undetected buffer overflow",  ustoa(num, 2, output, 16, 0) < 0 );

    num = 10;
    result = "A";
    ustoa(num, 11, output, 16, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = 0xffff;
    result = "0000FFFF";
    ustoa(num, 16, output, 16, 8);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = 10;
    result = "10";
    ustoa(num, 10, output, 16, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = 21;
    result = "1A";
    ustoa(num, 11, output, 16, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = 35;
    result = "Z";
    ustoa(num, 36, output, 16, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = 0x001A;
    result = "00000022";
    ustoa(num, 12, output, 16, 8);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );


	return 0;
}

/*@null@*/
static const char *test_sstoa(  ) {
    short num = 0;
    char *result;
    char output[17];

    num = -1;
    result = "FFFF";
    sstoa(num, 16, output, 17, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = -1;
    result = "1111111111111111";
    sstoa(num, 2, output, 17, 0);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

    num = -1;
    result = "00031B13";
    sstoa(num, 12, output, 17, 8);
	mu_assert( "error, result and output don't match", strcmp(result, output) == 0 );

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_ustoa );
	mu_run_test( test_sstoa );

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
