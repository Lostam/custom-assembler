#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    long long value;
    struct node* next;
} Node;

void append_node(Node **, long long);
void fibonacci_series_linked_list(const int , Node **);
void write_linked_list_to_file(Node *, const char *, const int);

#endif /* FIBONACCI_H */