#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


/*
  * make expression node 
*/
ast* ast_new_operation(char* op, ast* left, ast* right) {

  ast* new = check_alloc(sizeof(ast));

  new->type = operation_type;
  new->u.operation.op = strdup(op);
  new->u.operation.left = left;
  new->u.operation.right = right;

  return new;

}
/*
  * make assignement node                         -------> maybe not necessary!!! (only for "for")
*/
ast* ast_new_assignment(char* name, ast* right){

  ast* new = check_alloc(sizeof(ast));

  new->type = assignment_type;
  new->u.assignment.name = name;
  new->u.assignment.right = right;

  return new;
}

/*
  * make statement block node 
*/
ast* ast_new_statements(statements* list){
ast* new = check_alloc(sizeof(ast));
new->type = statements_type;
new->u.statements = list;
return new;
}

statements* new_statements(ast* stmt, statements* list){
  statements *element = check_alloc(sizeof(statements));
  element->stmt = stmt;
  element->prec = list;
  return element;
}
/*
  * make "for" node with range (i .. j), condition and statements as children
*/
ast* ast_new_for(char* it, ast* range, ast* stmt){

  ast* new = check_alloc(sizeof(ast));

  new->type = for_type;
  new->u.for_stmt.range = range;
  new->u.for_stmt.statements = stmt;
  new->u.for_stmt.iterator = strdup(it);
  
  return new;

}

/*
  * make "if" node 
*/
ast* ast_new_if(ast* cond, ast* if_branch, ast* else_branch){

  ast* new = check_alloc(sizeof(ast));

  new->type = if_type;
  new->u.if_stmt.cond = cond;
  new->u.if_stmt.if_branch = if_branch;
  new->u.if_stmt.else_branch = else_branch;
  
  return new;

}

/*
  * make range node 
*/
ast* ast_new_range(ast* l, ast* r){
  
  ast* new = check_alloc(sizeof(ast));
  new->type = range_type;
  new->u.range.node = "range";
  new->u.range.left = l;
  new->u.range.right = r;
  return new;
}

ast* ast_new_number(int number) {
  ast* new = check_alloc(sizeof(ast));
  new->type = number_type;
  new->u.number = number;
  return new;
}

ast* ast_new_id(char* id) {
  
  ast* new = check_alloc(sizeof(* new));
  (new->u).id = check_alloc(strlen(id) + 1);
  strcpy( (new->u).id , id);
  new->type = id_type;
  return new;
}

ast* ast_new_parallel(char* op, clock** clk, ast* statements){
  ast* new = check_alloc(sizeof(* new));
  if (!strcmp(op, "finish")){
    new->type = finish_type;
    new->u.finish_stmt.clocks = clk;
    new->u.finish_stmt.stmt = statements;
  }
  else if (!strcmp(op, "assync")){
    new->type = assync_type;
    new->u.assync_stmt.stmt = statements;
    new->u.assync_stmt.clocks = clk;
  }
  return new;
}

ast* ast_new_advance(char* id){
  ast* new = check_alloc(sizeof(* new));
  new->type = advance_type;
  (new->u).advance = check_alloc(strlen(id) + 1);
  strcpy( (new->u).advance , id);
  return new;
}

static void statements_print(statements* statements, int indent){
  if (statements != NULL){
    statements_print(statements->prec, indent);
    ast_print(statements->stmt, indent);
  }
}

void ast_print(ast* ast, int indent) {

  for (int i = 0; i < indent; i++)
    printf("    ");

  switch (ast->type){

    case id_type :
      printf("%s\n", ast->u.id);
      break;

    case number_type : 
      printf("%d\n", ast->u.number);
      break;

    case operation_type :
      printf("%s", ast->u.operation.op);
      printf("\n");
      ast_print(ast->u.operation.left, indent + 1);
      ast_print(ast->u.operation.right, indent + 1);
      break;

    case assignment_type :
      printf("%s = \n", ast->u.assignment.name);
      ast_print(ast->u.operation.right, indent + 1);
      break;

    case while_type :
      printf("\n");
      ast_print(ast->u.while_stmt.statements, indent + 1);
      break;

    case statements_type :
      printf("statements\n");
      statements_print(ast->u.statements, indent);
      break;

    case if_type : 
      printf("if \n");
      ast_print(ast->u.if_stmt.cond, indent + 1);
      //printf("\n");
      ast_print(ast->u.if_stmt.if_branch, indent + 1);
      if (ast->u.if_stmt.else_branch){
        ast_print(ast->u.if_stmt.else_branch, indent + 1);
      }
      break;

    case for_type : 
      printf("for \n");
      printf("    %s", ast->u.for_stmt.iterator);
      printf("\n");
      ast_print(ast->u.for_stmt.range, indent + 1);
      ast_print(ast->u.for_stmt.statements, indent + 1);
      break;

    case range_type : 
      printf("range \n");
      ast_print(ast->u.range.left, indent + 1);
      ast_print(ast->u.range.right, indent + 1);
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
  }
}
