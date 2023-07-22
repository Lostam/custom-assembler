#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"

// todo :: rewrite method later
// todo :: change name to remove first word
char *removeFirstNWords(const char *string, int N)
{
    const char *start = strdup(string);

    // Skip leading whitespaces
    while (isspace(*start))
    {
        start++;
    }

    // Find the end of the first word
    const char *end = start;
    while (*end != '\0' && !isspace(*end))
    {
        end++;
    }

    // Skip trailing whitespaces after the first word
    while (isspace(*end))
    {
        end++;
    }

    // Calculate the length of the remaining string
    size_t length = strlen(end);

    // Allocate memory for the result
    char *result = (char *)malloc((length + 1) * sizeof(char));
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the remaining string to the result
    strcpy(result, end);

    return result;
}

int get_number_of_words(const char *original)
{
    if (is_empty(original))
    {
        return 0;
    }
    char *copy = strdup(original);
    int count = 0;

    char *token = strtok(copy, " ");

    while (token != NULL)
    {
        count++;
        token = strtok(NULL, " ");
    }

    return count;
}

int is_whole_number(const char *original)
{
    if (is_empty(original))
    {
        return 0;
    }
    char *string = strdup(original);
    if (!isdigit(*string) && strncmp(string, "-", 1))
    {
        return 0;
    }
    string++;
    while (*string != '\0')
    {
        if (!isdigit(*string))
        {
            return 0;
        }
        string++;
    }

    return 1;
}

int is_empty(const char *string){
    return ((!string) || !string[0]);
}