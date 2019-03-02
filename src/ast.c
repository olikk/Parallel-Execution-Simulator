#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include <assert.h>

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

ast* ast_new_operation(char* op, ast* left, ast* right) {

  ast* new = check_alloc(sizeof(ast));

  new->type = operation_type;
  new->u.operation.op = strdup(op);
  new->u.operation.left = left;
  new->u.operation.right = right;

  return new;

}

ast* ast_new_assignment(char* name, ast* right){

  ast* new = check_alloc(sizeof(ast));

  new->type = assignment_type;
  new->u.assignment.name = name;
  new->u.assignment.right = right;

  return new;
}

ast* ast_new_statements(int, ast*);

ast* ast_new_while(ast*, ast*);

ast* ast_new_for(char* it, ast* range, ast* stmt){

  ast* new = check_alloc(sizeof(ast));

  new->type = for_type;
  new->u.for_stmt.range = range;
  new->u.for_stmt.statements = stmt;
  new->u.for_stmt.iterator = strdup(it);
  
  return new;

}

ast* ast_new_if(ast* cond, ast* if_branch, ast* else_branch){

  ast* new = check_alloc(sizeof(ast));

  new->type = if_type;
  new->u.if_stmt.cond = cond;
  new->u.if_stmt.if_branch = if_branch;
  new->u.if_stmt.else_branch = else_branch;
  
  return new;

}

ast* ast_new_range(int l, int r){
  
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

void ast_print(ast* ast, int indent) {
  for (int i = 0; i < indent; i++)
    printf("    ");
  //printf("%u", ast->type);
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
      
    case while_type :
      printf("\n");
      ast_print(ast->u.while_stmt.statements, indent + 1);
      break;
      //statements_type, 
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
      // last_element
  }
}
