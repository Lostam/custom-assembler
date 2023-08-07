#ifndef MY_MACROS_H
#define MY_MACROS_H

/**
 *  Defines general constant to be used in the project
*/

#define MAX_LINE_LENGTH 80
#define BASE_MEMORY_COUNTER 100
#define MAXIMUM_ALLOWED_NUMBER 65535 // as 1111111111111111 is the largest allowed word
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define RESERVED_WORDS "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", ".data", ".string", ".entry", ".extern", "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"

#endif /* MY_MACROS_H */