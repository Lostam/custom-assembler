#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main( int argc, char *argv[] ) {
    assembler as;

	if ( argc < 2 ) {
		fprintf( stderr, "Missing arguments\n" );
		exit( EXIT_FAILURE );
	}

    as = newAssembler(  );

	while ( *++argv != NULL ) {
		int ret;
		ret = assemble( as, *argv );
		if ( ret != 0 ) {
            deleteAssembler( as );
			return EXIT_FAILURE;
        }
        printf("%s compiled successfully\n", *argv);
	}

    deleteAssembler( as );
    
	return 0;
}
