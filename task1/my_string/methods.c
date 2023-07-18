#include <stdio.h>
#include <string.h>

int my_strcmp(const char *s1, const char *s2) {
    int i = 0;
    while((s1[i] && s2[i]) && s1[i] == s2[i]) {
        i++;
    }
    if (s1[i] == s2[i]) {
        return 0;
    }
    if (!s1[i]) {
        return 0-s2[i];
    }
    if (!s2[i]) {
        return s1[i];
    }
    return s1[i] - s2[i];
}

int my_strncmp(const char *s1, const char *s2, int n) {
    int i = 0;
    while((s1[i] && s2[i]) && s1[i] == s2[i] && i<n-1) {
        i++;
    }
    if (s1[i] == s2[i]) {
        return 0;
    }
    if (!s1[i]) {
        return 0-s2[i];
    }
    if (!s2[i]) {
        return s1[i];
    }
    return s1[i] - s2[i];
}

int my_strchr(char *str, char character) {
    int i = 0;
    while(str[i]) {
        if (str[i] == character) {
            return i;
        }
        i++;
    }
    return -1;
}

