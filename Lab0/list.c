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

// Allocate space for new_node and set its word and length
struct List_node_s* create_node(char* word, size_t len) {
    struct List_node_s* new_node
            = (struct List_node_s*) malloc(sizeof(struct List_node_s));
    (*new_node).word = word;
    (*new_node).length = len;
    return new_node;
}

// Inserts a node to the linked list right before a node with a larger word length
// or at the end if there is no word larger.
void insert_sorted(struct List_node_s** head_ptr, struct List_node_s* new_node) {
    // Special case: node gets inserted at head
    if (*head_ptr == NULL || (*new_node).length < (**head_ptr).length) {
        //new_node->next = *head_ptr;
        //*head_ptr = new_node;

        (*new_node).next = (*head_ptr);
        (*head_ptr) = new_node;
    } else { // Normal case: node gets inserted somewhere in middle or end
        struct List_node_s* current = *head_ptr;
        // Find spot to insert into list (either at end of list or right before something longer)
        while ((*current).next != NULL && (*new_node).length >= (*(*current).next).length) {
            current = (*current).next;
        }
        // Insert into list
        (*new_node).next = (*current).next;
        (*current).next = new_node;
    }
}

// Iterate through linked list, printing the word for each node
void print_list(struct List_node_s* head) {
    // initialize cur to be the head of the list
    struct List_node_s* current = head;
    // check if current is null, print its word, then update current reference
    while(current != NULL) {
        printf("%s\n", (*current).word);
        current = (*current).next;
    }
    // reached end of list, nothing left to print
}

// Iterate through linked list and clear memory at each node
void clear_LL_memory(struct List_node_s* head) {
    // set current variable to be reference to head and initialize a temp reference
    struct List_node_s* current = head;
    struct List_node_s* temp;
    // check if current is null, then save its next node to a temp variable so current can be cleared out
    while(current != NULL) {
        temp = (*current).next;
        free(current);
        current = temp;
    }
    // reached end of list
}

int main(int argc, char *argv[]){
    // Create head node pointer and new node pointer
    struct List_node_s* head = NULL;
    struct List_node_s* new_node;
    for (int i = 1; i < argc; i++) {
        // Get next word from argv[] and determine its length
        char *word = argv[i];
        size_t len = strlen(word);
        // Allocate space and set variables for new_node
        new_node = create_node(word, len);
        // Put node into position in LL based on length
        // Pass in address of head to be able to change which node is called head, rather than modifying head
        insert_sorted(&head, new_node);
    }
    // Print the list of nodes to terminal and clear the memory allocated for the linked list
    print_list(head);
    clear_LL_memory(head);
    return 0;
}