#ifndef _STOA_H_
#define _STOA_H_

/* Convert n to a string according to the given base.  Supports base 2 - 36
 * using letters A - Z to represent digits 10 - 36.
 *
 * This function prints a max of 16 bits of worth of data so that if short is
 * larger than 16 bits on your hardware the value will be truncated to the
 * lowest 16 bits.
 *
 * @param n number to convert
 * @param base Base to convert to.
 * @param s char array to store string into
 * @param size The size of the array. The function will store up to size - 1
 * bytes of number followed by '\0'.  
 * @param minWidth The number will be left padded with zeros up to minWidth.
 * @returns 0 on success, or non zero on overflow error.
 */
int ustoa( unsigned short n, int base, char s[] , int size, int minWidth);

/* same as ustoa but makes sure negative numbers are represented using 2's
 * complement.
 */
int sstoa( short n, int base, char s[] , int size, int minWidth);

#endif
