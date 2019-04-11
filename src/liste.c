#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

/*
  * check if memory was allocated
  * 
*/
void* check_alloc(size_t sz){
    void* result = malloc(sz);
    if(!result)
    {
        printf("alloc failed\n");
        exit(1);
    }
    return result;
}

clock* push_clock(clock *c, char* id){
    
    clock *element = check_alloc(sizeof(clock));
    element->id = (char*)check_alloc((strlen(id)+1) * sizeof(char));
    strcpy(element->id, id);
    element->prec = c;
    
    return element;
}

void print_clock(clock *c){
        while(c)
          {
             printf("%s ",c->id);
             c = c->prec;
          }
}