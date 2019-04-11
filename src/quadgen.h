#ifndef __QUADGEN_H
#define __QUADGEN_H

#include "ast.h"


typedef struct quad{
	int label;	//label
	char *op;	//Operator
	struct symbol* arg1;	//First argument
	struct symbol* arg2;	//Second argument
	struct quad* next;		//next quad in the NULL-termina
} quad;

quad* quad_gen(int la, char* op, symbol* arg1, symbol* arg2);
quad* quad_add(quad* quad_dest,quad* qd);

void ast_to_quad(ast* a, quad* quadlist);

#endif
