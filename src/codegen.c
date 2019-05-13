#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "pile.h"

int code_num = 0;
int stack_num = 0;
pile* stack;
code* code_gen(int la, char* op, char* a1, int a2){

	code* new=malloc(sizeof(struct code));

	new->label = la;
	new->op = op;
	new->arg1 = a1;
	new->arg2 = a2;
	new->next = NULL;

	return new;
}

code* code_add( code* codelist, code* code) {

	if(codelist == NULL){
        codelist = code;
	} else {
		struct code* pointer = codelist;
		while(pointer->next != NULL){
			pointer = pointer->next;
		}
		pointer->next = code;
	}
    return codelist;
}

//search code structure knowing label number
code* goto_code(code* codelist, int label){
  code* temp = codelist;
  while (temp!=NULL){
    if (temp->label == label)
      return temp;
    temp = temp->next;
  }
  return temp;
}

static code* statements_to_code(statements* statements, code* codelist){
  if (statements != NULL){
    codelist = statements_to_code(statements->prec, codelist);
    codelist = ast_to_code(statements->stmt, codelist);
  }
  return codelist;
}

code* ast_to_code(ast* ast, code* codelist) {
  switch (ast->type){

    case id_type :
      printf("id code\n");
      codelist = code_add(codelist, code_gen(code_num++,"PUSH", ast->u.id, stack_num++));
      break;

    case basic_type :
      printf("basic code\n");
      codelist = code_add(codelist, code_gen(code_num++,"BASIC", ast->u.id, 0));
      break;

    case number_type : 
      printf("number code\n");
      codelist = code_add(codelist, code_gen(code_num++,"PUSH", NULL, ast->u.number));
      break;

    case operation_type :
      printf("operation code\n");
      if (strcmp(ast->u.operation.op, "+") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"ADD", NULL, 0));
      } else if (strcmp(ast->u.operation.op, "-") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"SUB", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "*") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"MUL", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "/") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"DIV", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "==") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"EQ", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "!=") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"NE", NULL, 0));
      }else if (strcmp(ast->u.operation.op, ">") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"GT", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "<") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"LT", NULL, 0));
      }else if (strcmp(ast->u.operation.op, ">=") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"GE", NULL, 0));
      }else if (strcmp(ast->u.operation.op, "<=") == 0){
        codelist = code_add(codelist, code_gen(code_num++,"LE", NULL, 0));
      }

      codelist = ast_to_code(ast->u.operation.right, codelist);
      codelist = ast_to_code(ast->u.operation.left, codelist);
      
      break;

    case assignment_type :
      break;

    case statements_type :
      printf("statements code\n");
      codelist = statements_to_code(ast->u.statements, codelist);
      printf("end of stmts\n");
      break;

    case if_type : 
      printf("if code\n");
      codelist = ast_to_code(ast->u.if_stmt.cond, codelist);
      code* else_code = code_gen(code_num++,"IFZERO", "GOTO", 0);
      codelist = code_add(codelist, else_code);
      codelist = ast_to_code(ast->u.if_stmt.if_branch, codelist);
      code* end_if_code = code_gen(code_num++,"ENDIF", "GOTO", 0);
      codelist = code_add(codelist, end_if_code);
      else_code->goto_label = code_num;
      if (ast->u.if_stmt.else_branch != NULL){
        codelist = ast_to_code(ast->u.if_stmt.else_branch, codelist);
      }
      end_if_code->goto_label = code_num;
      printf("end of if\n");
      break;

    case for_type : 
      printf("for code\n");
      codelist = ast_to_code(ast->u.for_stmt.range, codelist);
      //SET i (range left)
      codelist = code_add(codelist, code_gen(code_num++,"SET", ast->u.for_stmt.iterator, stack_num++));
      int start_loop_label = code_num;
      // EVAL i (operation) (range right)
      codelist = code_add(codelist, code_gen(code_num++,"LE", NULL, 0)); //a faire
      //jump to the end of loop
      code* if_false = code_gen(code_num++,"IFZERO", "GOTO", 0);
      codelist = code_add(codelist, if_false);
      codelist = ast_to_code(ast->u.for_stmt.statements, codelist);
      codelist = code_add(codelist, code_gen(code_num++,"INC",ast->u.for_stmt.iterator , 0));  
      //repeat loop
      code* jump_start = code_gen(code_num++,"LOOP", "GOTO", 0);
      
      codelist = code_add(codelist, jump_start);
      jump_start->goto_label = start_loop_label;
      if_false->goto_label = code_num;
      printf("end of for\n");      
      break;

    case range_type : 
      printf("range code\n");
      codelist = ast_to_code(ast->u.range.right, codelist); 
      codelist = ast_to_code(ast->u.range.left, codelist);
      break;

    case finish_type :
      printf("finish code\n");
      codelist = code_add(codelist, code_gen(code_num++,"FINISH",NULL , 0));
      clock* clock_finish = ast->u.finish_stmt.clocks;  //NULL pointer 
      while(clock_finish != NULL){
        codelist = code_add(codelist, code_gen(code_num++,"CLOCK_CREATE", clock_finish->id , 0)); 
        clock_finish = clock_finish->prec;
      }
      codelist = code_add(codelist, code_gen(code_num++,"END_PREAMBLE",NULL , 0));
      codelist = ast_to_code(ast->u.finish_stmt.stmt, codelist);
      codelist = code_add(codelist, code_gen(code_num++,"END_FINISH",NULL , 0));
      printf("end of finish\n");
      break;

    case async_type :
      printf("async code\n");
      code* suite = code_gen(code_num++,"ASYNC", "GOTO" , 0);
      codelist = code_add(codelist, suite);
      clock* ca = ast->u.async_stmt.clocks; 
      while(ca != NULL){
        codelist = code_add(codelist, code_gen(code_num++,"CLOCK_REGISTER", ca->id , 0));
        ca = ca->prec;
      }
      code* prev_activity = code_gen(code_num++,"END_PREAMBLE","GOTO" , 0);
      codelist = code_add(codelist, prev_activity);
      suite->goto_label = code_num;
      codelist = ast_to_code(ast->u.async_stmt.stmt, codelist);
      codelist = code_add(codelist, code_gen(code_num++,"END_ASYNC",NULL , 0));
      prev_activity->goto_label = code_num;
      printf("end of async\n");
      break;

    case advance_type :
      printf("advance code\n");
      codelist = code_add(codelist, code_gen(code_num++,"ADVANCE",ast->u.advance , 0));
      break;
  }
  return codelist;
}

void code_print(code* codelist){
  code* p = codelist;
  while ( p != NULL){
    printf("%d. ", p->label);
    printf("%s ", p->op);
    if (p->arg1 != NULL) printf("%s ", p->arg1);
    if ((strcmp(p->op, "SET")==0)||(strcmp(p->op, "PUSH")==0)) printf("%d ", p->arg2);
    if ((p->arg1 != NULL) && (strcmp(p->arg1, "GOTO") == 0)){
      printf("%d ", p->goto_label);
    }
    printf("\n");
    p = p->next;
    
  }
}