#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "complex.h"


void print(Complex *comp) {
    double real = comp->real;
    double imaginary = comp->imaginary;
    if (real == 0 &&  imaginary == 0) {
       printf("0\n");
    }
    if (real != 0 &&  imaginary == 0) {
       printf("%.2f\n", real);
    }
    if (real == 0 &&  imaginary != 0) {
       printf("%.2fi\n", imaginary);
    }
    if (real != 0 &&  imaginary != 0) {
       if (imaginary > 0) {
            printf("%.2f + %.2fi\n", real, imaginary);
       }
       if (imaginary < 0) {
            printf("%.2f - %.2fi\n", real, imaginary * -1);
       }
    }
}

void add(Complex *first, Complex *second) {
    Complex *result = malloc(sizeof(Complex));
    if (result == NULL){
        printf("Failed allocation memory for struct Complex");
    }
    result->real = first->real + second->real;
    result->imaginary = first->imaginary + second->imaginary;
    printf("add result is : ");
    print(result);
}

void subtract(Complex *first, Complex *second) {
    Complex *result = malloc(sizeof(Complex));
    if (result == NULL){
        printf("Failed allocation memory for struct Complex");
    }
    result->real = first->real - second->real;
    result->imaginary = first->imaginary - second->imaginary;
    printf("subtract result is : ");
    print(result);
}

void multiply_comp_real(Complex *comp, int real) {
    Complex *temp_complex = malloc(sizeof(Complex));
    if (temp_complex == NULL){
        printf("Failed allocation memory for struct Complex");
        return;
    }
    temp_complex->real=real;
    temp_complex->imaginary=0;
    multiply(comp, temp_complex);
    free(temp_complex);
}

void multiply_comp_img(Complex *comp, int imaginary) {
    Complex *temp_complex = malloc(sizeof(Complex));
    if (temp_complex == NULL){
        printf("Failed allocation memory for struct Complex");
        return;
    }
    temp_complex->real=0;
    temp_complex->imaginary=imaginary;
    multiply(comp, temp_complex);
    free(temp_complex);
}


void multiply(Complex *first, Complex *second) {
    Complex *result = malloc(sizeof(Complex));
    if (result == NULL){
        printf("Failed allocation memory for struct Complex");
    }
    result->real = (first->real * second->real) - (first->imaginary * second->imaginary);
    result->imaginary = (first->real * second->imaginary) + (first->imaginary * second->real);
    printf("multiply result is : ");
    print(result);
}

void absolute(Complex *complex) {
    double *abs = malloc(sizeof(double*));
    *abs = sqrt((pow(complex->real,2)) + pow(complex->imaginary,2));
    printf("abs is %f\n", *abs);
}