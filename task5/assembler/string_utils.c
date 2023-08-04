#include "string_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "macros.h"
// change file name
// todo :: rewrite method later
// todo :: change name to remove first word
char *remove_first_n_words(const char *string, int N) {
    const char *start = strdup(string);

    // Skip leading whitespaces
    while (isspace(*start)) {
        start++;
    }

    // Find the end of the first word
    const char *end = start;
    while (*end != '\0' && !isspace(*end)) {
        end++;
    }

    // Skip trailing whitespaces after the first word
    while (isspace(*end)) {
        end++;
    }

    // Calculate the length of the remaining string
    size_t length = strlen(end);

    // Allocate memory for the result
    char *result = (char *)malloc((length + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the remaining string to the result
    strcpy(result, end);

    return result;
}

int get_number_of_words(const char *original, char delimiter) {
    if (is_empty(original)) {
        return 0;
    }
    char *copy = strdup(original);
    int count = 0;

    char *token = strtok(copy, &delimiter);

    while (token != NULL) {
        printf("Token is : [%s] for word : [%s]\n", token, copy);
        count++;
        token = strtok(NULL, &delimiter);
    }

    return count;
}
// todo :: support +
int is_whole_number(const char *original) {
    if (is_empty(original)) {
        return 0;
    }
    char *string = strdup(original);
    if (!isdigit(*string) && strncmp(string, "-", 1)) {
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


// rewrite
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

int is_reserved_word(const char *string) {
    const char* reserved_words[] = {RESERVED_WORDS};
    size_t size = ARRAY_SIZE(reserved_words);
    for (size_t i = 0; i < size; i++) {
        if (strcmp(string, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// todo :: use me for string validation
// bool is_valid_ascii(const char* str) {
//     while (*str != '\0') {
//         if (*str < 0 || *str > 127) {
//             return false;
//         }
//         str++;
//     }
//     return true;
// }

// todo :: padding char with null
// כל עוד זה לא מפריע לי האם אני יכול לוותר על הריפוד של char * ב NULL בסוף ?
// או שזה בגדר מוסכמות השפה?
// כי נגיד לי זה נראה בזבוז סתם כל עוד אני יודע בוודאות שאני לא צריך.

// rewrite
// Function to trim leading and trailing whitespaces from a string