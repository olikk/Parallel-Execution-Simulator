#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include <assert.h>

static void* check_alloc(size_t sz)
{
    void* result = calloc(sz, 1);
    if(!result)
    {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }
}

ast* ast_new_operation(char* op, ast* left, ast* right) {

  ast* new = check_alloc(sizeof(ast));

  new->type = operation_type;
  new->u.operation.op = strdup(op);
  new->u.operation.left = left;
  new->u.operation.right = right;

  return new;

}

ast* ast_new_assignement(char* name, ast* right){

  ast* new = malloc(sizeof(ast));

  new->type = assignement_type;
  new->u.assignement.name = name;
  new->u.assignement.right = right;

  return new;
}

ast* ast_new_statements(int, ast*);

ast* ast_new_while(ast*, ast*);

ast* ast_new_if(ast* cond, ast* if_branch, ast* else_branch){

  ast* new = malloc(sizeof(ast));

  new->type = if_type;
  new->u.if_stmt.cond = cond;
  new->u.if_stmt.right.if_branch = if_branch;
  new->u.if_stmt.right.else_branch = else_branch;
  
  return new;

}

ast* ast_new_number(int number) {
  ast* new = malloc(sizeof(ast));
  new->type = strdup("number");
  new->u.number = number;
  return new;
}

ast* ast_new_id(char* id) {
  ast* new = malloc(sizeof(ast));
  new->type = strdup("id");
  new->u.id = strdup(id);
  return new;
}

void ast_print(ast* ast, int indent) {
  for (int i = 0; i < indent; i++)
    printf("    ");
  printf("%s", ast->type);
  if (strcmp(ast->type, "number") == 0)
    printf(" (%d)\n", ast->u.number);
  else if (strcmp(ast->type, "id") == 0)
    printf(" (%s)\n", ast->u.id);
  else {
    printf("\n");
    ast_print(ast->u.operation.left, indent + 1);
    ast_print(ast->u.operation.right, indent + 1);
  }
}
