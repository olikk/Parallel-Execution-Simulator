#include "ast.h"
#include "semantic_check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

symbol* symbol_add (symbol * tab, char * id, symbol_type type){
	symbol* new = (symbol *)malloc(sizeof(struct symbol));
	new->id = id;
	new->type = type;
	new->next = tab;
	
	return new;
}
symbol * symbol_lookup (symbol * tab, char * id){
	symbol * current = tab;
	while (current != NULL){
		if (strcmp(current->id, id) == 0){
			return current;
		}
		current = current->next;
	}
	free(current);
	return NULL;
}

static void statements_semantic_check(statements* statements, symbol* tab){
  if (statements != NULL){
	statements_semantic_check(statements->prec, tab);
	semantic_check(statements->stmt, tab);
  }
}

int semantic_check(ast* ast, symbol* tab){
	int return_code = 0;
	if(ast!=NULL){
		switch (ast->type){

			case id_type :
				printf("id check\n");
				if (!symbol_lookup(tab, ast->u.id)){
					fprintf(stderr, "error: use of undeclared identifier \"%s\" \n", ast->u.id);
					return_code = 1;
				}
			break;

			case basic_type:
				printf("basic check\n");
			break;

			case operation_type :
				printf("operation check\n");
				semantic_check(ast->u.operation.left, tab);
				semantic_check(ast->u.operation.right, tab);
			break;

			case assignment_type :
				printf("%s = \n", ast->u.assignment.name);
				semantic_check(ast->u.operation.right, tab);
			break;

			case statements_type :
				printf("statements check\n");
				statements_semantic_check(ast->u.statements, tab);  
			break;

			case if_type : 
				printf("if check\n");
				semantic_check(ast->u.if_stmt.cond, tab);
				semantic_check(ast->u.if_stmt.if_branch, tab);
				if (ast->u.if_stmt.else_branch){
					semantic_check(ast->u.if_stmt.else_branch, tab);
				}
			break;

			case for_type : 
				printf("for check\n");
				tab = symbol_add(tab, ast->u.for_stmt.iterator, counter_type);
				semantic_check(ast->u.for_stmt.range, tab);
				semantic_check(ast->u.for_stmt.statements, tab);
			break;

			case range_type : 
				printf("range check\n");
				semantic_check(ast->u.range.left, tab);
				semantic_check(ast->u.range.right, tab);
			break;

			case finish_type :
				printf("finish check\n");
				clock* t = ast->u.finish_stmt.clocks;
				while(t){
					tab = symbol_add(tab, t->id, clock_type);
					t = t->prec;
				}
				//free(t);
				semantic_check(ast->u.finish_stmt.stmt, tab);
			
			break;

			case async_type :
				printf("async check\n");
				symbol* new_tab = NULL;
				clock* temp = ast->u.async_stmt.clocks;
				while(temp != NULL){
						// check if a clock exists
					if (symbol_lookup(tab,  temp->id)){
						//copy it to the new table
						new_tab = symbol_add(new_tab, temp->id, clock_type);
					} else {
						printf("error: use of undeclared clock \"%s\" in async\n",temp->id );
						return_code = 1;
					}   
					temp = temp->prec;
				}
				//free(temp);
				//copy all counter symbols to the new table
				while (tab != NULL){
					if (tab->type == counter_type){
						new_tab = symbol_add(new_tab, tab->id, counter_type);
					}
					tab = tab->next;
				}
				semantic_check(ast->u.async_stmt.stmt, new_tab);
			break;

			case advance_type :
				printf("advance check\n");
				if (!symbol_lookup(tab, ast->u.advance)){
					fprintf(stderr, "error: use of undeclared clock \"%s\" in advance\n", ast->u.advance);
					return_code = 1;
				}
			break;

			default: break;
		}
	}
	return return_code;
}