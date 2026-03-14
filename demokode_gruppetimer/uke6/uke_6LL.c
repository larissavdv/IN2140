/* 
Kode fra Cbra, løsningsforslag uke5:
https://github.uio.no/IN2140v2/Cbra/blob/master/uke05/losningsforslag/doubly_linked_solution.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***DOBBELTLENKET LISTE****/


typedef struct node {
    struct node *next;
    struct node *prev;
    int value;
} node_t;

node_t *head, *tail;


void set_between(node_t *left, node_t *middle, node_t *right) {
    left->next = middle;
    right->prev = middle;
    middle->next = right;
    middle->prev = left;
}

void remove_node(node_t *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void push(int number) {
    node_t *node = malloc(sizeof(node_t));
    node->value = number;
    set_between(head, node, head->next);
}

int pop(void) {
    node_t *node = head->next;
    remove_node(node);
    int number = node->value;
    free(node);
    return number;
}

void print_list() {
    node_t *temp = head->next;
    int i = 1;
    while (temp != tail) {
        printf("Value of node %d is %d\n", i++, temp->value);
        temp = temp->next;
    }
}

void free_list() {
    node_t *next, *temp = head->next;
    while (temp != tail) {
        next = temp->next;
        free(temp);
        temp = next;
    }
    free(head);
    free(tail);
}

int main(void) {
    head = malloc(sizeof(node_t));
    tail = malloc(sizeof(node_t));
    head->next = tail;
    tail->prev = head;

    for (size_t i = 0; i < 8; i++) {
        push(i * 33);
    }

    print_list();

    printf("Popping value %d\n", pop());
    printf("Popping value %d\n", pop());
    printf("Popping value %d\n", pop());

    print_list();

    free_list();

    return EXIT_SUCCESS;
}