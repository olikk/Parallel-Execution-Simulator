#ifndef EXEC_H
#define EXEC_H

typedef struct state{
    //pile de compteurs de boucle englobants 
    //une reference aux clocks accesibles (entrants)
    //une reference au finish englobant le plus proche
    //une pile par l'evaluation des expression (eval) ?deja en ast_to_code
} state;

state* exec_code(code*);

#endif