#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"

pile* new_node(int num){
    pile* new = (pile*) malloc(sizeof(struct pile));
    if (new == NULL){
        printf("malloc error: pile is empty\n");
        exit(1);
    }

    new->num= num;
    new->next = NULL;
    return new;
}

pile* push(pile* root, int num) { 

    pile* node = new_node(num); 
    node->next = root; 
    root = node; 
    printf("value pushed\n");
    return root;
} 

int pop(pile* root) { 
    
    if (root == NULL){
        printf("pop: pile is empty\n");
        exit(1);
    }
    pile* temp = root; 
    root = root->next; 
    int popped = temp->num; 

    return popped; 
}