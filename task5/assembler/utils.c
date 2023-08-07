#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"


Node *new_node(char *content) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        error("Memory allocation failed.");
        exit(1);
    }
    
    node->content = strdup(content);
    node->next = NULL;
    return node;
}

StringLinkedList *new_linked_list() {
    StringLinkedList *l_list = (StringLinkedList *)malloc(sizeof(StringLinkedList));
    if (l_list == NULL) {
        error("Memory allocation failed.");
        exit(1);
    }
    
    l_list->head = NULL;
    l_list->tail = NULL;
    l_list->size = 0;
    return l_list;
}


void add_node_to_list(StringLinkedList *l_list, Node *node) {
    if (l_list == NULL) {
        error("Must initlize linked list before usage");
        exit(1);
    }
    
    if (l_list->head == NULL) {
        l_list->head = node;
        l_list->tail = node;
        return;
    }
    l_list->tail->next = node;
    l_list->tail = node;
    l_list->size++;
}

void add_string_to_list(StringLinkedList *l_list, char *content) {
    Node *node = new_node(content);
    add_node_to_list(l_list, node);
}


void free_linked_list(StringLinkedList *l_list) {
    while (l_list->head != NULL) {
        Node *tmp = l_list->head->next;
        free_node(l_list->head);
        l_list->head = tmp;
    }
    free(l_list);
}


void free_node(Node *node) {
    free(node->content);
    free(node);
}