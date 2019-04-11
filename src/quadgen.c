#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadgen.h"

int quad_num = 0;
quad* quad = NULL;

quad* quad_gen(int la, char* op, symbol* a1, symbol* a2){
	quad* new=malloc(sizeof(struct quad));
	new->label=la;
	new->op=op;
	new->arg1=a1;
	new->arg2=a2;
	new->next=NULL;
	return new;
}

quad* quad_add( quad* quad_dest, quad* qd){
	if(quad_dest == NULL){
        quad_dest = qd;
	}
	else {
		quad* p= quad_dest;
		while(p->next != NULL){
			p=p->next;
		}
		p->next=qd;
	}
    return quad_dest;
}

void ast_to_quad(ast* ast, quad * quad) {

  switch (ast->type){

    case id_type :
        //add new id to tds
        //symbol* new = symbol_add(symbol_table, ast->u.id));
        break;

    case basic_type :
        printf("%s\n", ast->u.id);
        quad = quad_add(quad, quad_gen( quad_num++,"BASIC", ast->u.id, NULL));
        break;

    case number_type : 
      break;

    case operation_type :
      quad = quad_add(quad, quad_gen( quad_num++, ast->u.operation.op, /* ?, ?, ? */));
      ast_to_quad(ast->u.operation.left, quad);
      ast_to_quad(ast->u.operation.right, quad);
      break;

    case assignment_type :
      break;

    case statements_type :
      break;

    case if_type : 
      break;

    case for_type : 
      break;

    case range_type : 
      break;

    case finish_type :
      
      break;

    case async_type :
      break;

    case advance_type :
      break;
  }
}