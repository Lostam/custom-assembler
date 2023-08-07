#include "string_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "macros.h"

char *remove_first_n_words(const char *string, int N) {
        if (N <= 0 || string == NULL || *string == '\0') {
        return strdup(string);
    }
    
    while (isspace((unsigned char)*string)) {
        string++;
    }

    int is_in_whitespace = 0;
    while (N > 0 && *string != '\0') {
        if (!is_in_whitespace && isspace((unsigned char)*string)) {
            N--;
            is_in_whitespace = 1;
        }
        else if (!isspace((unsigned char)*string)) {
            is_in_whitespace = 0;
        }
        string++;
    }

    while (isspace((unsigned char)*string)) {
        string++;
    }

    return strdup(string);
}


int get_number_of_words(const char *original, char *delimiter) {
    if (is_empty(original)) {
        return 0;
    }
    char *copy = strdup(original);
    int count = 0;

    char *token = strtok(copy, delimiter);

    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }
    free(copy);

    return count;
}

int is_whole_number(const char *original) {
    if (is_empty(original)) {
        return 0;
    }
    char *string = strdup(original);
    if (!isdigit(*string) && strncmp(string, "-", 1) && strncmp(string, "+", 1)) {
        return 0;
    }
    string++;
    while (*string != '\0') {
        if (!isdigit(*string)) {
            return 0;
        }
        string++;
    }

    return 1;
}

int is_empty(const char *string) {
    return ((!string) || !string[0]);
}

char *trim_spaces(char *string) {
    char *end;

    while (isspace((unsigned char)*string)) {
        string++;
    }

    if (*string == '\0') {
        return string;
    }

    end = string + strlen(string) - 1;
    while (end > string && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return string;
}

char *get_first_word(const char *string) {
    while (*string && isspace(*string)) {
        string++;
    }

    const char *end = string;
    while (*end && !isspace(*end)) {
        end++;
    }

    size_t length = end - string;

    char *first_word = (char *)malloc(length + 1);
    if (first_word == NULL) {
        error("Memory allocation error");
        exit(1);
    }
    strncpy(first_word, string, length);
    first_word[length] = '\0';

    return first_word;
}

int is_reserved_word(const char *string) {
    const char *reserved_words[] = {RESERVED_WORDS};
    size_t size = ARRAY_SIZE(reserved_words);
    for (size_t i = 0; i < size; i++) {
        if (strcmp(string, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_valid_ascii(unsigned char str) {
    return str > 0 && str < 127;
}

int starts_with(const char *string, const char *prefix) {
   if(strncmp(string, prefix, strlen(prefix)) == 0) return 1;
   return 0;
}