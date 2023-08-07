#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char *to_binary(unsigned int,unsigned int);

int main(int argc, char *argv[]) {
    int arr[10] = {1, 2, 3,4,5,6,7,8,9,20};
    int i,j;
    printf("number is\n");
    for (i=0;i<10;i++) {
        char *result = to_binary(arr[i], 4);
        printf("number is %d and binary is %s\n", arr[i], result);
        printf("\n");
    }

}

char *to_binary(unsigned int num, unsigned int length) {
   char* binary = (char*)malloc((length+1) * sizeof(char));
    if (binary == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }

    binary[length] = '\0'; // Null-terminate the string

    for (int i = length - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return binary;
  }