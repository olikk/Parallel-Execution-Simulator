#ifndef __SEMANTIC_CHECK_H
#define __SEMANTIC_CHECK_H

typedef enum { counter_type, clock_type } symbol_type;
typedef struct symbol{
    symbol_type type;
    char* id;
    struct symbol * next;
} symbol;

symbol* init_s();
symbol * symbol_add (symbol *, char *, symbol_type);
symbol * symbol_lookup (symbol *, char *);
int semantic_check(ast*, symbol* tab);

#endif