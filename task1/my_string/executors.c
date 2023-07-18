#include <stdio.h>
#include <string.h>
#include "methods.h"

void execute_my_strcmp();
void execute_my_strncmp();
void execute_my_strchr();

void execute_my_strcmp() {
    char str1[80], str2[80];
    printf("Please provide the first string, max length is 80 chars\n");
    scanf("%s", str1);
    printf("First string is %s\n" , str1);
    printf("Please provide the second string, max length is 80 chars\n");
    scanf("%s", str2);
    printf("Second string is %s\n" , str2);
    printf("Will compare \"%s\" to \"%s\"\n\n" , str1, str2);
    printf("Result from original strcmp method : %d\n", strcmp(str1,str2));
    printf("Result from my_strcmp method : %d\n", my_strcmp(str1,str2));
}

void execute_my_strncmp() {
    char str1[80], str2[80];
    int n;
    printf("Please provide the first string, max length is 80 chars\n");
    scanf("%s", str1);
    printf("First string is %s\n" , str1);
    printf("Please provide the second string, max length is 80 chars\n");
    scanf("%s", str2);
    printf("Second string is %s\n" , str2);
    printf("Please provide the number of chars to compare\n");
    scanf("%d", &n);
    printf("Will compare \"%s\" to \"%s\" up to the %d char\n\n" , str1, str2, n);
    printf("Result from original strncmp method : %d\n", strncmp(str1, str2, n));
    printf("Result from my_strncmp method : %d\n", my_strncmp(str1, str2, n));
}

void execute_my_strchr() {
    char str[80], character;
    printf("Please provide the a string, max length is 80 chars\n");
    scanf("%s", str);
    printf("The chosen string is \"%s\"\n" , str);
    printf("Please provide a char to find in %s string\n", str);
    scanf(" %c", &character);
    printf("Letter provided is \"%c\"\n" , character);
    printf("Will return the position of \"%c\" in \"%s\" if found, will return -1 otherwise\n\n" , character, str);
    printf("Result from original strchr method : %s\n", strchr(str, character));
    printf("Result from my_strchr method : %d\n", my_strchr(str, character));
}