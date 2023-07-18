#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mycomp.h"


#define COMMA ','
#define BLOCK_SIZE 100

int main() {
    Complex *complex_a = malloc(6);
    if (complex_a == NULL){
        printf("Failed allocation memory for struct Complex");
        return 0;
    }
    int i;
    for (i= 0; i < 6; i++) {
        complex_a[i].real = 0;
        complex_a[i].imaginary = 0;
    }
    char *user_input = malloc(BLOCK_SIZE);
    printf("Please start entering your command,\n enter 'stop' anytime to exit\n\n");
    char **input_array = malloc(BLOCK_SIZE * sizeof(char*));

    int is_eof = 0;
    while(!is_eof) {
        printf("---------------------------------\n\n");
        printf("Ready for new complex command\n\n");
        char *res = fgets(user_input, BLOCK_SIZE, stdin);
        if (res==NULL) {
            free(res);
            is_eof = 1;
            continue;
        }
        user_input[strcspn(user_input, "\r\n")] = 0;
        printf("Command received : %s\n", user_input);
        if (startWith(user_input, "read_comp")) {
            InputTypeEnum order[5] = {LETTER,SEPARATOR, NUMBER,SEPARATOR, NUMBER};
            if (validate_input(user_input,order, 5, "read_comp ", input_array)){
                read_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input, "print_comp")) {
            InputTypeEnum order[1] = {LETTER};
            if (validate_input(user_input,order, 1, "print_comp ", input_array)) {
                print_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"add_comp")) {
            InputTypeEnum order[3] = {LETTER, SEPARATOR, LETTER};
            if (validate_input(user_input,order, 3, "add_comp ", input_array)) {
                add_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"sub_comp")) {
            InputTypeEnum order[3] = {LETTER,SEPARATOR, LETTER};
            if (validate_input(user_input,order, 3, "sub_comp ", input_array)) {
                sub_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"mult_comp_real")) {
            InputTypeEnum order[3] = {LETTER,SEPARATOR, NUMBER};
            if (validate_input(user_input,order, 3, "mult_comp_real ", input_array)) {
                mult_comp_real(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"mult_comp_img")) {
            InputTypeEnum order[3] = {LETTER,SEPARATOR, NUMBER};
            if (validate_input(user_input,order, 3, "mult_comp_img ", input_array)) {
                mult_comp_img(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"mult_comp_comp")) {
            InputTypeEnum order[3] = {LETTER,SEPARATOR, LETTER};
            if (validate_input(user_input,order, 3, "mult_comp_comp ", input_array)) {
                mult_comp_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input,"abs_comp")) {
            InputTypeEnum order[1] = {LETTER};
            if (validate_input(user_input,order, 1, "abs_comp ", input_array)) {
                abs_comp(input_array, complex_a);
            }
            continue;
        }
        if (startWith(user_input, "stop")) {
            InputTypeEnum order[0] = {};
            if (validate_input(user_input,order, 0, "stop", input_array)) {
                free(complex_a);
                stop();
            }
            continue;
        }
        else {
            printf("Invalid format: Undefined command name\n");
        }
    }
    printf("WARNING : EOF detected, the program should only be terminated with the stop command");
    free(complex_a);
    free(user_input);
    return 0;
}

void read_comp(char **input_array, Complex *arr) {
    int index = letter_to_index(*input_array[0]);
    arr[index].real=atof(input_array[1]);
    arr[index].imaginary=atof(input_array[2]);
    print(&arr[index]);
}

void print_comp(char **input_array, Complex *arr) {
    int index = letter_to_index(*input_array[0]);
    printf("%c is: ", *input_array[0]);
    print(&arr[index]);
}

void add_comp(char **input_array, Complex *arr) {
     int index_1 = letter_to_index(*input_array[0]);
     int index_2 = letter_to_index(*input_array[1]);
     add(&arr[index_1], &arr[index_2]);
}

void sub_comp(char **input_array, Complex *arr) {
     int index_1 = letter_to_index(*input_array[0]);
      int index_2 = letter_to_index(*input_array[1]);
      subtract(&arr[index_1], &arr[index_2]);
}

void mult_comp_real(char **input_array, Complex *arr) {
     int index = letter_to_index(*input_array[0]);
      multiply_comp_real(&arr[index], atof(input_array[1]));
}

void mult_comp_img(char **input_array, Complex *arr) {
     int index = letter_to_index(*input_array[0]);
     multiply_comp_img(&arr[index], atof(input_array[1]));
}

void mult_comp_comp(char **input_array, Complex *arr) {
     int index_1 = letter_to_index(*input_array[0]);
     int index_2 = letter_to_index(*input_array[1]);
     multiply(&arr[index_1], &arr[index_2]);
}

void abs_comp(char **input_array, Complex *arr) {
     int index = letter_to_index(*input_array[0]);
     absolute(&arr[index]);
}

void stop() {
    printf("Detected stop command, aborting program\n");
    exit(0);
}