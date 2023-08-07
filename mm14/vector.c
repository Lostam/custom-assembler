#include <stdlib.h>
#include <assert.h>

#include "vector.h"

typedef union {
	void *vp;
	unsigned short usint;
} vecData;

struct vector_t {
	int capacity, size;
	vecData *dp;
};

/* initialSize should always be at least one */
vector newVector( int initialSize ) {
	vector vec = malloc( sizeof( *vec ) );

	vec->capacity = initialSize ? initialSize : 1;
	vec->size = 0;
	vec->dp = malloc( sizeof( vecData ) * vec->capacity );

	return vec;
}

/* If deleteFunc is NULL then you have to clean the objects up yourself,
 * otherwise deleteFunc will be called on each object pointed to by the
 * container.
 */
void deleteVector( vector v, deleteFunc df ) {
	int i;

	if ( df != NULL )
		for ( i = 0; i != vecGetSize( v ); i++ )
			df( v->dp[i].vp );

	free( v->dp );
	free( v );
}

/* Behaves same as deleteVector without deallocating it's storage */
void vectorClear( vector v, deleteFunc df ) {
	int i;

	if ( df != NULL )
		for ( i = 0; i != vecGetSize( v ); i++ )
			df( v->dp[i].vp );

	v->size = 0;
}

/* Returns the current number of elements in the container.
 * This is not nessacarily the same thing as the capacity of the container.
 */
int vecGetSize( vector v ) {
	return v->size;
}

/* Functions for dealing with object pointers */
int vecAppendObject( vector v, void *ob ) {

	if ( v->size == v->capacity ) {
		v->capacity <<= 1;
		v->dp = realloc( v->dp, sizeof( vecData ) * v->capacity );
	}
	v->dp[v->size++].vp = ob;

	return 0;
}

/* If you attempt to get an element past the end of the vector you'll get NULL
 */
void *vecGetObject( vector v, int index ) {

	if ( index > v->size )
		return NULL;

	return v->dp[index].vp;
}

/* Function for dealing with unsigned shorts */
int vecAppendUS( vector v, unsigned short data ) {

	if ( v->size == v->capacity ) {
		v->capacity <<= 1;
		v->dp = realloc( v->dp, sizeof( vecData ) * v->capacity );
	}
	v->dp[v->size++].usint = data;

	return 0;
}

/* If you attempt to get an element past the end of the vector you'll get
 * undefined behaviour since every possible return value is valid */
unsigned short vecGetUS( vector v, int index ) {
	return v->dp[index].usint;
}
