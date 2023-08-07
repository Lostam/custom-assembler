#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>

/**
 * @brief Removes the first N words from a string and returns the resulting string
*/
char* remove_first_n_words(const char* , int);
/**
 * @brief Returns the first word in a string
*/
char* get_first_word(const char *);
/**
 * @brief Returns the number of words in a string by the delimiter in the second parameter
*/
int get_number_of_words(const char *, char *);
/**
 * @brief Check if a string is a whole number (also check if starts with +/-)
*/
int is_whole_number(const char *);
/**
 * @brief Checks if a string is empty or null
*/
int is_empty(const char *);
/**
 * @brief Checks if a char is valid ascii char
*/
int is_valid_ascii(unsigned char);
/**
 * @brief Removes the leading and trailing whitespaces from a string and returns the resulting string
*/
char *trim_spaces(char *);
/**
 * @brief Checks if a given string is one of the reserved words
*/
int is_reserved_word(const char *);
#endif /* STRING_UTILS_H */