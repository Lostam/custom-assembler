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
}

void add_string_to_list(StringLinkedList *l_list, char *content) {
    Node *node = new_node(content);
    add_node_to_list(l_list, node);
}


void free_linked_list(StringLinkedList *l_list) {
    while (l_list->head != NULL) {
        Node *tmp = l_list->head->next;
        free(l_list->head);
        l_list->head = tmp;
    }
    free(l_list);
}


void free_node(Node *node) {
    free(node->content);
    free(node);
}


/**
 * #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 100 // Size of the hash table

// Define a struct to represent each node in the linked list
typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

// Define the hash function
unsigned hash(char* s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

// Function to create a new node
Node* createNode(char* key, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->key = strdup(key); // Duplicate the key to store in the node
        newNode->value = value;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to insert a key-value pair into the hash table
void insert(Node** hashtable, char* key, int value) {
    unsigned index = hash(key);
    Node* newNode = createNode(key, value);

    if (newNode == NULL) {
        // Memory allocation failed
        return;
    }

    if (hashtable[index] == NULL) {
        // No collision, insert the new node directly
        hashtable[index] = newNode;
    } else {
        // Collision occurred, add the new node at the beginning of the linked list
        newNode->next = hashtable[index];
        hashtable[index] = newNode;
    }
}

// Function to retrieve the value associated with a key from the hash table
int get(Node** hashtable, char* key) {
    unsigned index = hash(key);
    Node* current = hashtable[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found, return the associated value
            return current->value;
        }
        current = current->next;
    }

    // Key not found, return an appropriate value (e.g., -1 to indicate an error)
    return -1;
}

// Function to free memory allocated for the hash table and its nodes
void freeHashTable(Node** hashtable) {
    for (int i = 0; i < HASHSIZE; i++) {
        Node* current = hashtable[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(hashtable);
}

int main() {
    Node** hashtable = (Node**)calloc(HASHSIZE, sizeof(Node*));

    // Insert some key-value pairs into the hash table
    insert(hashtable, "apple", 5);
    insert(hashtable, "banana", 10);
    insert(hashtable, "orange", 7);

    // Retrieve values from the hash table using keys
    printf("apple: %d\n", get(hashtable, "apple"));   // Output: apple: 5
    printf("banana: %d\n", get(hashtable, "banana")); // Output: banana: 10
    printf("orange: %d\n", get(hashtable, "orange")); // Output: orange: 7

    // Free the memory allocated for the hash table
    freeHashTable(hashtable);

    return 0;
}

*/