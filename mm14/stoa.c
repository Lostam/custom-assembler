#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "stoa.h"

/* reverse: reverse string s in place */
static void reverse( char s[] ) {
	int c, i, j;
	for ( i = 0, j = strlen( s ) - 1; i < j; i++, j-- ) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int sstoa( short n, int base, char s[], int size, int minWidth ) {

	/* make sure a negative number is represented by a 2's complement bit
	 * pattern regardless of what cpu we're running on. */
	unsigned short u;
	if ( n < 0 ) {
		u = ( unsigned short )-n;
		u = ~u;
		u += 1;
	} else
		u = ( unsigned short )n;

	return ustoa( u, base, s, size, minWidth );
}

int ustoa( unsigned short n, int base, char s[], int size, int minWidth ) {
	int i = 0;

	if ( base < 2 || base > 36 )
		return -2;

#if USHRT_MAX > 65535
	/* Truncate to 16 bits in case sizeof(short)*CHAR_BIT > 16 on this
	 * platform */
	n &= 0xFFFF;
#endif

	/* generate digits in reverse order */
	do {
		unsigned short temp;
		/* check for buffer overflow */
		if ( i == size - 1 )
			return -1;
		temp = n % base;
		if ( temp >= 10 ) {
			temp -= 10;
			s[i++] = ( char )( temp + 'A' );
		} else
			s[i++] = ( char )( temp + '0' );
	} while ( ( n /= base ) > 0 );

	while ( i < minWidth )
		s[i++] = '0';

	s[i] = '\0';
	reverse( s );

	return i;
}
