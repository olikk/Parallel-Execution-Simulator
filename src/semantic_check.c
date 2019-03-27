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
    if(node!=NULL){
        switch (ast->type){

            case id_type :
            printf("%s\n", ast->u.id);
            break;

            case number_type : 
            printf("%d\n", ast->u.number);
            break;

            case operation_type :
                semantic_check(ast->u.operation.left);
                semantic_check(ast->u.operation.right);
            break;

            case assignment_type :
                printf("%s = \n", ast->u.assignment.name);
                semantic_check(ast->u.operation.right);
            break;

            case statements_type :
                printf("statements\n");
                statements_semantic_check(ast->u.statements);  // à faire!!!
            break;

            case if_type : 
                printf("if \n");
                semantic_check(ast->u.if_stmt.cond);
            //printf("\n");
                semantic_check(ast->u.if_stmt.if_branch);
                if (ast->u.if_stmt.else_branch){
                    semantic_check(ast->u.if_stmt.else_branch);
                }
            break;

            case for_type : 
                printf("for \n");
                printf("    %s", ast->u.for_stmt.iterator);
                printf("\n");
                semantic_check(ast->u.for_stmt.range);
                semantic_check(ast->u.for_stmt.statements);
            break;

            case range_type : 
                printf("range \n");
                semantic_check(ast->u.range.left, indent + 1);
                semantic_check(ast->u.range.right, indent + 1);
            break;

            case finish_type :
            printf("finish\n");
            ast_print(ast->u.finish_stmt.stmt, indent + 1);
            for (int i=0; i<=indent;i++){
                printf("    ");
            }
            print_clock(* ast->u.finish_stmt.clocks);
            break;

            case assync_type :
            printf("assync\n");
            ast_print(ast->u.assync_stmt.stmt, indent + 1);
            for (int i=0; i<=indent;i++){
                printf("    ");
            }
            print_clock(* ast->u.assync_stmt.clocks);
            break;

            case advance_type :
            printf("advance %s\n", ast->u.advance);
            break;

            default: break;
        }

        semantic_check(node, tab){}
    }
}