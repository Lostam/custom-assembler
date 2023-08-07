#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "fp_defs.h"

/* A flexible array class which can store either pointers to objects of
 * integral values, depending on which interface you use. 
 */

typedef struct vector_t *vector;

/* initialSize should always be at least one */
vector newVector( int initialSize );

/* If deleteFunc is NULL then you have to clean the objects up yourself,
 * otherwise deleteFunc will be called on each object pointed to by the
 * container.
 */
void deleteVector( vector v, deleteFunc df );

/* Returns the current number of elements in the container.
 * This is not nessacarily the same thing as the capacity of the container.
 */
int vecGetSize( vector v );

/* Behaves same as deleteVector without deallocating it's storage */
void vectorClear( vector v, deleteFunc df );

/* Functions for dealing with object pointers */
int vecAppendObject( vector v, void *ob );

/* If you attempt to get an element past the end of the vector you'll get NULL
 */
void *vecGetObject( vector v, int index );

/* Function for dealing with unsigned shorts */
int vecAppendUS( vector v, unsigned short data );
/* If you attempt to get an element past the end of the vector you'll get
 * undefined behaviour since every possible return value is valid */
unsigned short vecGetUS( vector v, int index );

#endif
