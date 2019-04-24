#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "ast.h"

//intermidiate code structure
typedef struct code{
	int label;	//label
	char *op;	//Operator
	char* arg1;	//First argument
	int arg2;	//Second argument
	int goto_label;
	struct code* next;		//next code in the NULL-termina
} code;

code* code_gen(int label, char* op, char* arg1, int arg2);
code* code_add(code* codelist, code* code);

code* ast_to_code(ast* a, code* codelist);
void code_print(code* codelist);

#endif
