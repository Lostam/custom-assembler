#ifndef UTILS_H
#define UTILS_H

typedef struct node_s Node;
typedef struct string_linked_list_s StringLinkedList;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node_s {
    char *content;
    Node *next;
};

struct string_linked_list_s {
    Node *tail;  
    Node *head;
    size_t size;
};

Node *new_node(char *);
StringLinkedList *new_linked_list();
void add_node_to_list(StringLinkedList *, Node *);
void add_string_to_list(StringLinkedList *, char *);
void free_linked_list(StringLinkedList *);
void free_node(Node *);


#endif /* UTILS_H */


