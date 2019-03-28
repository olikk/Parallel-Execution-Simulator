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

void semantic_check(ast* ast, symbol** tab){
    if(ast!=NULL){
        switch (ast->type){

            case id_type :
            printf("%s\n", ast->u.id);
            break;

            case number_type : 
            printf("%d\n", ast->u.number);
            break;

            case operation_type :
                semantic_check(ast->u.operation.left, tab);
                semantic_check(ast->u.operation.right, tab);
            break;

            case assignment_type :
                printf("%s = \n", ast->u.assignment.name);
                semantic_check(ast->u.operation.right, tab);
            break;

            case statements_type :
                printf("statements\n");
                //statements_semantic_check(ast->u.statements);  // à faire!!!
            break;

            case if_type : 
                printf("if \n");
                semantic_check(ast->u.if_stmt.cond, tab);
            //printf("\n");
                semantic_check(ast->u.if_stmt.if_branch, tab);
                if (ast->u.if_stmt.else_branch){
                    semantic_check(ast->u.if_stmt.else_branch, tab);
                }
            break;

            case for_type : 
                printf("for \n");
                printf("    %s", ast->u.for_stmt.iterator);
                printf("\n");
                semantic_check(ast->u.for_stmt.range, tab);
                semantic_check(ast->u.for_stmt.statements, tab);
            break;

            case range_type : 
                printf("range \n");
                semantic_check(ast->u.range.left, tab);
                semantic_check(ast->u.range.right, tab);
            break;

            case finish_type :
            printf("finish\n");
            semantic_check(ast->u.finish_stmt.stmt, tab);
            print_clock(* ast->u.finish_stmt.clocks);
            break;

            case assync_type :
            printf("assync\n");
            semantic_check(ast->u.assync_stmt.stmt, tab);
            print_clock(* ast->u.assync_stmt.clocks);
            break;

            case advance_type :
            if (!symbol_lookup(*tab, ast->u.advance))
                fprintf(stderr, "error: use of undeclared clock\n");
            break;

            default: break;
        }
    }
}