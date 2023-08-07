#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../string_utils.h"

int main(int argc, char *argv[]) {
    char *string = "100.00";
    printf("expecting 1 and got : %d\n", is_whole_number("123"));
    printf("expecting 1 and got : %d\n", is_whole_number("1"));
    printf("expecting 1 and got : %d\n", is_whole_number("-1"));
    printf("expecting 1 and got : %d\n", is_whole_number("+1"));
    printf("expecting 1 and got : %d\n", is_whole_number("-0"));
    printf("expecting 0 and got : %d\n", is_whole_number("123.1"));
    printf("expecting 0 and got : %d\n", is_whole_number("_123.1"));
    printf("expecting 0 and got : %d\n", is_whole_number("-10.1"));
    printf("expecting 0 and got : %d\n", is_whole_number("1-0.1"));
    printf("expecting 0 and got : %d\n", is_whole_number("--100"));
    printf("expecting 0 and got : %d\n", is_whole_number("11-1"));
    printf("expecting 0 and got : %d\n", is_whole_number("11-1"));
    char *nully = NULL;
    printf("IS EMPTY \n");
    printf("expecting 1 and got : %d\n", is_empty(""));
    printf("expecting 1 and got : %d\n", is_empty(nully));
    printf("expecting 1 and got : %d\n", is_empty(NULL));
    printf("expecting 0 and got : %d\n", is_empty("aaaa"));
    
    printf("remove first x words \n");
    printf("expecting A and got : %s\n", remove_first_n_words("mcro A",1));
    printf("expecting '' and got : %s\n", remove_first_n_words("mcro",1));
    printf("expecting C and got : %s\n", remove_first_n_words("Adfgs Basdas C",2));
    printf("expecting A and got : %s\n", remove_first_n_words("   mcro A",1));
    printf("expecting C    D and got : %s\n", remove_first_n_words("       AZZ     Basda     C     D",2));

}