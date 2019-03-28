#include "ast.h"
#include "semantic_check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

symbol** init_s(){
    symbol *s = (symbol *)malloc(sizeof(struct symbol));
    if (s == NULL){
        exit(1);
    }
    s->type = 0;
    s->next = NULL;

    return &s;
}

symbol * symbol_add (symbol ** tab, char * id, int type){
    symbol* new = (symbol *)malloc(sizeof(struct symbol));
    new->id = id;
    if (!tab){
        tab = init_s();
        *tab = new;
    } else {
        new->next = *tab;
        *tab = new->next;
    }
    return new;
}
symbol * symbol_lookup (symbol * tab, char * id){
    symbol * current = tab;
    while (current != NULL){
        if (current->id == id){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void statements_semantic_check(statements* statements, symbol** tab){
  if (statements != NULL){
    statements_check(statements->prec, tab);
    semantic_check(statements->stmt, tab);
  }
}

int semantic_check(ast* ast, symbol** tab){
    int return_code = 0;
    if(ast!=NULL){
        switch (ast->type){

            case id_type :
                printf("id check\n");
                if (!symbol_lookup(*tab, ast->u.id)){
                    fprintf(stderr, "error: use of undeclared identifier \"%s\" \n", ast->u.id);
                    return_code = 1;
                }
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
                *tab = symbol_add(tab, ast->u.for_stmt.iterator, counter);
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
                while(*(ast->u.finish_stmt.clocks){
                    *tab = symbol_add(tab, *(ast->u.finish_stmt.clocks)->id, clock);
                    *(ast->u.finish_stmt.clocks) = *(ast->u.finish_stmt.clocks) -> prec;
                }
                semantic_check(ast->u.finish_stmt.stmt, tab);
            
            break;

            case async_type :
                printf("async check\n");
                if (ast->u.async_stmt.clocks != NULL){
                    symbol** new_tab = NULL;
                    *new_tab = (symbol *)malloc(sizeof(struct symbol));
                    *new_tab = NULL;
                    while(*(ast->u.async_stmt.clocks){
                        // check if a clock exists
                        if (symbol_lookup(*tab,  *(ast->u.async_stmt.clocks)->id)){
                            //copy it to the new table
                            *new_tab = symbol_add(new_tab, *(ast->u.async_stmt.clocks)->id, clock);
                        } else {
                            printf("error: use of undeclared clock \"%s\" in async\n",*(ast->u.async_stmt.clocks)->id );
                            return_code = 1;
                        }
                        
                        *(ast->u.async_stmt.clocks) = *(ast->u.async_stmt.clocks) -> prec;
                    }
                    //copy all counter symbols to the new table
                    while (*tab != NULL){
                        if ((*tab)->type == counter){
                            *new_tab = symbol_add(new_tab, (*tab)->id, counter);
                        }
                        *tab = (*tab)->next;
                    }
                    //now original tab points to the new one
                    tab = new_tab;

                }
                semantic_check(ast->u.async_stmt.stmt, tab);
            break;

            case advance_type :
                printf("advance check\n");
                if (!symbol_lookup(*tab, ast->u.advance)){
                    fprintf(stderr, "error: use of undeclared clock \"%s\" in advance\n", ast->u.advance);
                    return_code = 1;
                }
            break;

            default: break;
        }
    }
    return return_code;
}