#ifndef __PILE_H
#define __PILE_H

typedef struct pile{
    int num;
    struct pile* next;
} pile;


pile* push(pile* root, int num);
int pop(pile* root);

#endif
