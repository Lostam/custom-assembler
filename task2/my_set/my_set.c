#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_set.h"

#define A_SIZE_INCREASE 10
#define ENLARGE_SIZE(a) a+= A_SIZE_INCREASE


int main() {
    MySet *my_set = malloc(sizeof(MySet));
    if (my_set == NULL){
        printf("Failed allocation memory for struct mySet");
        return 0;
    }
    my_set->size = 10;
    my_set->numbers = malloc(my_set->size * sizeof(int));
    my_set->current_index = 0;
    int next_value = '\0';
    printf("Please start entering your integer inputs,\ntrigger EOF anytime to print the results\n\n");
    while(scanf("%d", &next_value) != EOF) {
        if (!is_in_set(my_set, next_value)) {
            add_to_array(my_set, next_value);
            my_set->current_index++;
        } else {
            printf("%d is already in set,will not add it\n", next_value);
        }
    }
    printf("EOF detected\n");
    printf("_________________________________\n");
    print_set(my_set);
}

int is_in_set(MySet *set, int item_to_find) {
    int i;
    for(i = 0 ; i < set->current_index; i++) {
        if (set->numbers[i] == item_to_find) {
            return 1;
        }
    }
    return 0;
}

void print_set(MySet *set) {
    printf("Printing provided set : \n");
    int i;
    for(i = 0 ; i < set->current_index; i++) {
        printf("Item in place %d is : %d\n", i, *(set->numbers + i));
    }
    printf("Finished printing the provided set!\n");
}

void add_to_array(MySet *set, int value) {
    /* 10-boom */
    if ((set->current_index + 1) == set->size) {
        ENLARGE_SIZE(set->size);
        printf("Resized array by %d to %d\n", A_SIZE_INCREASE, set->size);
        set->numbers = (int *) realloc(set->numbers, set->size * sizeof(int));
    }
    printf("Adding %d to set\n", value);
    (set->numbers)[set->current_index] = value;
}