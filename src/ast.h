#ifndef __AST_H
#define __AST_H

#include "liste.h"

typedef struct statements{             
      struct ast * stmt;
      struct statements * prec;
    } statements;

typedef struct ast {

  enum {id_type, number_type, basic_type, advance_type, operation_type, assignment_type, for_type, range_type, statements_type, if_type, finish_type, async_type} type;
  union {

    int number;

    char* id;

    statements * statements; //for a block of statements

    char* advance;  //advance clock statement

    struct {             // for "expression" statements
      struct ast* left;
      struct ast* right;
      char* op;
    } operation;

    struct {               // for "assignment" statements
      char*name;
      struct ast* right;
    } assignment;

      struct {              // for "for" statements
        char* iterator;
        struct ast* range;
        struct ast* statements;
      } for_stmt;

      struct {             // for a range in "for" statements
        char* node;
        struct ast* left;
        struct ast* right;
      } range;

      struct {            // for "if/else" statements
        struct ast* cond;
        struct ast* if_branch;
        struct ast* else_branch;
      } if_stmt;

      struct {            // for "finish" statements
        clock* clocks;
        struct ast* stmt;
      } finish_stmt;

      struct {            // for "async" statements
        clock* clocks;
        struct ast* stmt;
      } async_stmt;
  } u;
} ast;


ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_assignment(char*, ast*);
ast* ast_new_statements(statements*);
statements* new_statements(ast*, statements*);
ast* ast_new_for(char*, ast*, ast*);
ast* ast_new_range(ast*, ast*);
ast* ast_new_if(ast*, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_advance(char*);
ast* ast_new_id(char*);
ast* ast_new_basic(char* );
ast* ast_new_parallel(char*, clock*, ast*);
void ast_print(ast*, int);

#endif