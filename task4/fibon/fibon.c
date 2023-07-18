#include <stdio.h>
#include <stdlib.h>
#include "fibon.h"


int main(int argc, char *argv[]) {
    int index;
    char* file_name=*++argv;
    if (argc == 1) {
        fprintf(stderr, "Error: missing output file path.\n");
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "Error : too many arguments provided, expecting only file path\n");
        return 1;
    }
    printf("Provided file path is : %s\n", file_name);
    printf("Please enter the index of the desired fibonacci number, the entered number must be non-negative whole: \n");
    scanf("%d", &index);

    if (index < 0) {
        fprintf(stderr, "Error: Invalid input, Please enter a non-negative whole number.\n");
        return 1;
    }

    printf("Received number is %d.\n", index);

    if (index >= 94) {
        fprintf(stderr, "Warn: The 94th Fibonacci number is 12200160415121876738, which representation is larger than 8 bytes and might be displayed incorrectly on some platforms.\n");
    }

    Node* head = NULL;

    fibonacci_series_linked_list(index, &head);
    write_linked_list_to_file(head, file_name, index);

    return 0;
}


void append_node(Node** current_node, long long new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Failed allocation memory for Node struct, exiting\n");
        exit(1);
    }
    new_node->value = new_data;
    new_node->next = NULL;

    if (*current_node == NULL) {
        *current_node = new_node;
        return;
    }

    new_node->next = *current_node;
    *current_node = new_node;
}

void fibonacci_series_linked_list(const int n, Node** current_node) {
    long long first = 0;
    long long second = 1;
    long long next;
    int i;
    Node* first_node;

    for (i = 0; i <= n; i++) {
        append_node(current_node, first);
        if (i == 0) {
            first_node = *current_node;
        }
        next = first + second;
        first = second;
        second = next;
    }
    first_node->next = *current_node;
}

void write_linked_list_to_file(Node* head, const char* filename, int size) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: failed opening file %s\n", filename);
        exit(1);
    }
    fprintf(file, "Printing the first %d numbers of the fibonacci sequence in a reverse order : \n", size);
    fprintf(file, "___________________________\n");
    int i;
    Node* temp = head;
    for (i = 0; i <= size; i++) {
        printf("%dth fibonacci number is : %lld\n", size-i, temp->value);
        fprintf(file, "%lld\n", temp->value);
        temp = temp->next;
    }

    printf("Linked list has been written to the file '%s'\n", filename);
    fclose(file);
}