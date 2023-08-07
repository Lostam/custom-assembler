#include <stdlib.h>
#include <string.h>

#include "symTabFacade.h"

static struct {
    const char* label;
    unsigned short offset;
} symTab[] = {
    { "foo", 0xA },
    { "bar", 0x9 },
    { "bob", 0x2 },
    { "baz", 0x0 }, /* this is an extern var */
};

int stfGetLabelAddress( symTabFacade stf, const char *label, unsigned short offset ) {
    int i;
    stf = NULL;  /* just to shut up the compiler */

    for ( i = 0; i != (sizeof(symTab)/sizeof(symTab[0])); i++ ) {
        if ( strcmp(symTab[i].label, label) == 0 ) 
            return (int)symTab[i].offset;
    }
    return -1;
}
