#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "symTabFacade.h"

int tests_run = 0;

/*@null@*/
static const char *test_stf(  ) {
    symTabFacade stf = newSymTabFacade();
    struct {
        const char* label;
        labelType type;
        unsigned short offset;
    } syms[] = {
        { "foo", labelData, 0xffff },
        { "bar", labelData, 0xffff },
        { "dead", labelEntry, 0xffff },
        { "dead", labelCode, 0xffff },
        { "baz", labelExtern, 0xffff },
        { "beef", labelData, 0xffff },
    };
    int i, dataSegBase = 0x5;

    for ( i = 0; i != (sizeof(syms)/sizeof(syms[0])); i++) {
        mu_assert("vecAppendObject failed",
            stfAddLabel( stf, syms[i].label, syms[i].type, syms[i].offset)
                == 0);
    }

    clearSymTabFacade( stf );

    for ( i = 0; i != (sizeof(syms)/sizeof(syms[0])); i++) {
        mu_assert("stfAddLabel failed",
            stfAddLabel( stf, syms[i].label, syms[i].type, syms[i].offset)
                == 0);
    }

    stfSetDataSegmentBase( stf,(unsigned short)dataSegBase );

    for ( i = 0; i != (sizeof(syms)/sizeof(syms[0])); i++) {
        int offset = stfGetLabelAddress( stf, syms[i].label, (unsigned short)i+1 );
        if ( syms[i].type == labelData )
            mu_assert("stfGetLabelAddress returned incorrect data offset", 
                offset == (syms[i].offset + dataSegBase) );
        else if ( syms[i].type == labelExtern )
            mu_assert("stfGetLabelAddress returned incorrect extern offset", 
                offset == 0 );
    }


    deleteSymTabFacade( stf );

	return 0;
}

/*@null@*/
static const char *all_tests(  ) {
	mu_run_test( test_stf );

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
