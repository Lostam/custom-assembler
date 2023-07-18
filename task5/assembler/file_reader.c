#include <stdio.h>
#include <stdlib.h>

int main(*fileName) {
    int i;
    int lines;
    int result;
    char *fileName;
    FILE *file;

    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: cannot open file '%s'\n", fileName);
        return 1;
    }

    char line[100];

    *lines = 0;
    while (fgets(line, 100, file) != NULL) {
        printf("%s", line);
        *lines = *lines + 1;
    }

    fclose(file);
    return 0;
}

