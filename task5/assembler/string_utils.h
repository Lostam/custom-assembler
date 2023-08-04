#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>

char* remove_first_n_words(const char* , int);
int get_number_of_words(const char *, char);
int is_whole_number(const char *);
int is_empty(const char *);
char *trim_spaces(char *);
int is_reserved_word(const char *);
#endif /* STRING_UTILS_H */