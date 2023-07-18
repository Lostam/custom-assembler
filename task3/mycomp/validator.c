#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "validator.h"

#define COMMA ','
#define BLOCK_SIZE 100

int validate_input(char *user_input, InputTypeEnum *order, int order_size, char *command_name, char **input_array) {
    char *copy = strdup(user_input);
    int copy_index = 0;
    if (strlen(copy) < strlen(command_name)) {
          printf("Invalid format: Missing parameter.\n");
          free(copy);
          return 0;
        }
    if (!startWith(copy, command_name)) {
      printf("Invalid format in command name: There should be no characters other than space after command name.\n");
      free(copy);
      return 0;
    }
    copy_index = strlen(command_name);
    char *part = NULL;
    int i;
    int parameter_index = 0;
    for (i = 0; i < order_size; i++) {
            part = malloc(BLOCK_SIZE);
            while(isspace(copy[copy_index])) {
                copy_index++;
            }
            get_next_section(copy, part, copy_index, order[i]);
            copy_index = copy_index + strlen(part);
            if (part[0] == '\0') {
                printf("Invalid format: Expected %d parameters and received %d.\n", order_size, i);
                free(copy);
                free(part);
                return 0;
            }

            InputTypeEnum type = order[i];
            if (type == LETTER) {
                input_array[parameter_index] = strdup(part);
                parameter_index++;
                if (!is_letter_valid(part, parameter_index)) {
                    free(copy);
                    free(part);
                    return 0;
                }
            } else if (type == NUMBER) {
                input_array[parameter_index] = strdup(part);
                parameter_index++;
                if (!is_numeric_valid(part, parameter_index)) {
                    free(copy);
                    free(part);
                    return 0;
                }
            } else if (type == COMMA) {
                 if (!is_comma_valid(part, parameter_index)) {
                     free(copy);
                     free(part);
                     return 0;
                 }
             }
        }
    if (copy[copy_index]!='\0') {
        printf("Invalid format: Extraneous text after end of command.\n");
        free(copy);
        if (part != NULL){
            free(part);
        }
        return 0;
    }
    free(copy);
    if (part != NULL){
        free(part);
    }
    return 1;
}

int is_letter_valid(char *section, int index) {
    if (strlen(section)!=1)  {
      printf("Invalid format in parameter %d: Wrong input length, expected a single letter from A-F.\n", index);
      return 0;
    }
    if (!isalpha(section[0]))  {
      printf("Invalid format in parameter %d: Wrong input, alphabetical letter expected.\n", index);
      return 0;
    }
    if (letter_to_index(section[0]) == -1)  {
      printf("Invalid format in parameter %d: Undefined complex variable.\n", index);
      return 0;
    }
    return 1;
}

int is_numeric_valid(char *section, int index) {
    if (!isnumeric(section))  {
      printf("Invalid format in parameter %d: Wrong input, numeric expected.\n", index);
      return 0;
    }
    return 1;
}

int is_comma_valid(char *section, int index) {
    if (strlen(section) != 1)  {
      printf("Invalid format in comma after the parameter %d: Wrong length, expected a single comma.\n", index);
      return 0;
    }

    if (section[0] != COMMA)  {
      printf("Invalid format in comma after the parameter %d:  Wrong input, expected comma.\n", index);
      return 0;
    }
    return 1;
}

int isnumeric(char *str) {
    int i = 0;

    if (str[i] == '-') {
        i++;
    }

    int has_digits = 0;
    int number_of_dots = 0;
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            has_digits = 1;
            i++;
        }
        else if(str[i] == '.' && number_of_dots == 0 && has_digits >0){
            number_of_dots++;
            i++;
        } else {
             printf("Found %c\n", str[i]);
            return 0;
        }
    }

    return has_digits;
}

int count_char(char* str, char chr) {
    int count = 0;
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == chr) {
            count++;
        }
    }

    return count;
}

int startWith(char *a, char *b) {
    if (strncmp(a, b, strlen(b)) == 0) {
        return 1;
    }
    return 0;
}

int letter_to_index(char letter) {
    switch(letter) {
        case 'A': return 0;
        case 'B': return 1;
        case 'C': return 2;
        case 'D': return 3;
        case 'E': return 4;
        case 'F': return 5;
        default: return -1;
    }
}

void get_next_section(char *origin, char *part, int copy_index, InputTypeEnum type) {
    int i=0;
    while(!isspace(origin[copy_index]) && origin[copy_index] != '\0' &&
     ((type != SEPARATOR && origin[copy_index]!=COMMA) || (type == SEPARATOR))) {
            part[i] = origin[copy_index];
            i++;
            copy_index++;
    }
}