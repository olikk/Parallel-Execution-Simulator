#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include <assert.h>

/*
  * check if memory was allocated
  * 
*/
void* check_alloc(size_t sz)
{
    void* result = malloc(sz);
    if(!result)
    {
        printf("alloc failed\n");
        exit(1);
    }
    return result;
}
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
ast* ast_new_statements(ast* result, ast* stmt){

  if(!result || result->type != statements_type)
    {
        result = check_alloc(sizeof(*result));
        result->type = statements_type;
        result->u.statements.count = 0;
        result->u.statements.statements = 0;
    } 
    result->u.statements.count++;
    result->u.statements.statements = realloc(result->u.statements.statements, result->u.statements.count*sizeof(*result->u.statements.statements));
    result->u.statements.statements[result->u.statements.count-1] = stmt;
    return result;
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
  
  ast* new = check_alloc(sizeof(ast));
  (new->u).id = check_alloc(strlen(id) + 1);
  strcpy( (new->u).id , id);
  new->type = id_type;
  return new;
}

ast* ast_new_parallel(char* op, char** clk, ast* stmts){
  ast* new = check_alloc(sizeof(ast));
  if (!strcmp(op, "finish")){
    new->type = finish_type;
    new->u.finish_stmt.clocks = clk;
    new->u.finish_stmt.stmt = stmts;
  }
  else if (!strcmp(op, "assync")){
    new->type = assync_type;
    new->u.assync_stmt.stmt = stmts;
    new->u.assync_stmt.clocks = clk;
  }
  return new;
}

clock* new_clocks(clock* clks, clock* c){

  if(!clks)
    {
        clks = check_alloc(sizeof(struct clks*));
        clks->clocks = check_alloc(sizeof(char*));
        strcpy(result[size], id);
        return result;
    } 
    for (size = 1; strcmp(result[size],"0")!=0 ; size++){}
    result[size] = malloc(strlen(id)+1);
    strcpy(result[size], id);
    return result;
}

void ast_print(ast* ast, int indent) {

  for (int i = 0; i < indent; i++)
    printf("    ");

  switch (ast->type){

    case id_type :
      printf("(%s)\n", ast->u.id);
      break;

    case number_type : 
      printf("(%d)\n", ast->u.number);
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
      printf("\n");
      for (int i = 0; i < ast->u.statements.count; i++)
        ast_print(ast->u.statements.statements[i], indent + 1);
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
  }
}
