#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../instruction_handler.h"

int main(int argc, char *argv[]) {
    char *string = "100.00";
    printf("expecting 1 and got : %d\n", is_register("@r0"));
    printf("expecting 1 and got : %d\n", is_register("@r1"));
    printf("expecting 1 and got : %d\n", is_register("@r2"));
    printf("expecting 1 and got : %d\n", is_register("@r3"));
    printf("expecting 1 and got : %d\n", is_register("@r4"));
    printf("expecting 1 and got : %d\n", is_register("@r5"));
    printf("expecting 1 and got : %d\n", is_register("@r6"));
    printf("expecting 1 and got : %d\n", is_register("@r7"));
    printf("expecting 0 and got : %d\n", is_register("r7"));
    printf("expecting 0 and got : %d\n", is_register("7"));
    printf("expecting 0 and got : %d\n", is_register("r@7"));
    printf("expecting 0 and got : %d\n", is_register("r7@"));
    printf("expecting 0 and got : %d\n", is_register("@r8"));
    printf("expecting 0 and got : %d\n", is_register("@r-1"));
    printf("expecting 0 and got : %d\n", is_register("@r11"));
    printf("expecting 0 and got : %d\n", is_register("@r1_"));
    printf("expecting 0 and got : %d\n", is_register("@ra"));
    
    printf("IS EMPTY \n");
}