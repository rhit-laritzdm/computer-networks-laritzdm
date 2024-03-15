/* File: list.c 
 * Author: Daniel Laritz
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List_node_s {
	int length;
	char* word;
    struct List_node_s* next;
} List_node;

void insert_sorted(struct List_node_s** head_ref, struct List_node_s* new_node) {
    struct List_node_s* current;
    /* Special case for the head end */
    if (*head_ref == NULL || (*head_ref)->length >= new_node->length) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else {
        /* Locate the node before the point of insertion */
        current = *head_ref;
        while (current->next != NULL && current->next->length < new_node->length) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

struct List_node_s* add_word(const char* word, size_t len) {
    struct List_node_s* new_node
            = (struct List_node_s*)malloc(sizeof(struct List_node_s));

    new_node->length = len;
    new_node->word = word;
    return new_node;
}

void print_list(struct List_node_s* head) {
    struct List_node_s* temp;
    while (temp != NULL) {
        printf("%s\n", temp->word);
        temp = temp->next;
    }
}

int main(int argc, char *argv[]){
    struct List_node_s* head = NULL;
    struct List_node_s* new_node;

    for (int i = 1; i < argc; i++) {
        char *word = argv[i];
        size_t len = strlen(word);
        new_node = add_word(word, len);
        insert_sorted(&head, new_node);
    }
    print_list(head);
    /*
    // start at i = 1 to skip ./list as a word
    struct List_node_s *head_node = NULL;
    struct List_node_s *new_node = NULL;

    struct List_node_s *head_node = NULL;
    for (int i = 1; i < argc; i++) {
        char *word = argv[i];
        size_t len = strlen(word);
        add_word(word, len);
    }
     */
    free(head);
    return 0;
}
