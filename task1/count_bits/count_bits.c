#include "count_bits.h"
#include <stdio.h>
#include <string.h>

int main(){
    unsigned long number;
    printf("Please provide your number\n");
    scanf("%lu", &number);
    printf("Number provided is : %lu\n", number);
    printf("Number in bit format is : ");
    to_binary(number);
    int pos_bits = count_bit(number);
    printf("Number of bits in even places is : %d\n", pos_bits);
}

int count_bit(unsigned long number) {
    int bitCounter = 0;
    int i=0;
    for (i=0 ; number != 0; number>>=1) {
        if (number & 01 && i % 2==0) {
            bitCounter++;
        }
        i++;
    }
    return bitCounter;
}

void to_binary(unsigned long n) {
    long i;
    printf("%d", 0);
    for (i = 1 << 30; i > 0; i = i / 2) {
      if((n & i) != 0) {
        printf("%d", 1);
      }
      else {
        printf("%d", 0);
      }
    }
    printf("\n");
  }
