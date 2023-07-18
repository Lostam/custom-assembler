#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"

// todo :: rewrite method later
// todo :: change name to remove first word
char *removeFirstNWords(const char *string, int N)
{
    const char* start = string;
    
    // Skip leading whitespaces
    while (isspace(*start)) {
        start++;
    }
    
    // Find the end of the first word
    const char* end = start;
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
    char* result = (char*)malloc((length + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    // Copy the remaining string to the result
    strcpy(result, end);
    
    return result;
}

int get_number_of_words(const char* string) {
    int count = 0;

    char* token = strtok(string, " ");

    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }

    return count;
}