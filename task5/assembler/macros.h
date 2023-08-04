#ifndef MY_MACROS_H
#define MY_MACROS_H

// Define your macros here
#define MAX_LINE_LENGTH 82
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define RESERVED_WORDS "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", ".data", ".string", ".entry", ".extern", "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7"
//התווית מקבלת את ערכה בהתאם להקשר בו היא מוגדרת. תווית בהנחיות string ,.data., תקבל את ערך מונה הנתונים )data counter( הנוכחי, בעוד שתווית המוגדרת בשורת הוראה תקבל את ערך מונה ההוראות )instruction counter( הנוכחי.

#endif // MY_MACROS_H