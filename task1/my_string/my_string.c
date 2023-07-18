#include <stdio.h>
#include <string.h>
#include "executors.h"

int main() {
    int method_number_to_run=-1;
    printf("Please choose a method to run: \n1: my_strcmp\n2: my_strncmp\n3: my_strchr\n");
    scanf("%d", &method_number_to_run);
    switch(method_number_to_run) {
        case 1: {
            printf("You chose to run my_strcmp\n\n");
            execute_my_strcmp();
            break;}
        case 2: {
            printf("You chose to run my_strncmp\n\n");
            execute_my_strncmp();
            break;
            }
        case 3: {
            printf("You chose to run my_strchr\n\n");
            execute_my_strchr();
            break;
        }
        default: return main();
    }
    return 1;
}