#include "codegen.h"
#include "exec.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

int activity_counter = 1;
int finish_counter = 0;


clocks* create_clock(char* id){
  clocks* new = check_alloc(sizeof(struct clocks));
  new->registered = check_alloc(sizeof(struct activity));
  new->blocked = check_alloc(sizeof(struct activity));
  new->id = id;
  return new;
}
clocks* add_clock(clocks* clock_list, char* id){
  clocks* new = create_clock(id);
  new->next = clock_list; 
  return new;
}
clocks* find_clock(char* id, clocks* clocks){
  struct clocks* temp = clocks;
  while(temp != NULL){
    if (strcmp(temp->id, id) == 0)
      return temp;
  }
  return NULL;
}
void clock_register(clocks* clk, activity* a){
  if (clk->registered != NULL){
    clk->registered->next = clk->registered;
    clk->registered = a;
  }else{
    clk->registered = a;
    clk->registered->next = NULL;
  }
}

void clock_deregister(clocks* clk, int activity){
  while (clk->registered != NULL){
    //activity* temp = clk->registered->next;           //???
    if (clk->registered->id == activity){
      clk->registered = clk->registered->next;
    }
  }
}

void clock_block(clocks* clk, activity* a){
  if (clk->blocked != NULL){
    clk->blocked->next = a;
    clk->blocked->next->next = NULL;
  }else{
    clk->blocked = a;
    clk->blocked->next = NULL;
  }
}

void clock_unblock(clocks* clk, int activity){
  while (clk->blocked != NULL){
    if (clk->blocked->id == activity){
      clk->blocked = clk->blocked->next;
    }
  }
}
activity* create_activity(int id){
  activity* new = check_alloc(sizeof(struct activity));
  new->id = id;
  new->next = NULL;
  return new;
}

activity* add_activity(activity* root, int id){
  activity* new = create_activity(id);
  new->next = root;
  return new;
}

activity* copy_activities(activity* liste){
  activity* current = liste;
  activity* new = NULL; 
  while (current != NULL){
    if (new == NULL){
      new = create_activity(current->id); 
    } else {
      new = add_activity(new, current->id);
    }
    new->program_counter = current->program_counter;
      //new->finish = 
      //new->clock_registered = 
      //new->pile = 
    current = current->next;
  }
  return new;
}

int compare_activities(activity* list1, activity* list2){
  activity* a = list1;
  activity* b = list2;
  while (a != NULL && b != NULL) 
    { 
        if (a->id != b->id) 
            return 0; 
  
        /* If we reach here, then a and b are  
        not NULL and their data is same, so  
        move to next nodes in both lists */
        a = a->next; 
        b = b->next; 
    } 
  
    // If linked lists are identical, then  
    // 'a' and 'b' must be NULL at this point. 
    return (a == NULL && b == NULL); 
}

finish* create_finish(int id){
  finish* new = check_alloc(sizeof(struct finish));
  new->id = id;
  new->next = NULL;
  return new;
}

finish* add_finish(finish* root, int id){
  finish* new = create_finish(id);
  new->next = root;
  return new;
}
ready* add_ready(ready* ready, activity* a){
  struct ready* new = check_alloc(sizeof(struct ready));
  new->activity  = a;
  new->next = ready;
  return new;
}

ready* remove_ready(ready* ready, int a){
  while (ready->activity != NULL){
    if (ready->activity->id == a){
      ready->activity = ready->activity->next;
    }
  }
  return ready;
}
/*void activity_free(activity* a){
  activity* temp;
  while (a!=NULL){
    temp = a->next;
    free(a);
    a = temp;
  }
}*/
void clock_free(clocks* clock){
  free(clock->registered);
  free(clock->blocked);
  //activity_free(clock->registered);
  //activity_free(clock->blocked);
  free(clock);
}

void advance(clocks* clk, ready* ready, activity* activity){
  clock_block(clk, activity);
  ready = remove_ready(ready,activity);
}

void run(code* codelist, state* program_state){
  while (program_state->ready != NULL){
    execute(program_state->running, codelist);
  }
}

void execute(activity* act, code* codelist){
  code* code = goto_label(codelist, act->program_counter);
  act->program_counter = act->program_counter + 1; //next instruction

  if (strcmp(code->op, "EVAL")==0) {

      /*printf("start compare oper\n");
      if (strcmp(ast->u.operation.op, "+") == 0){
        stack = push(stack, (pop(stack) + pop(stack)));
        printf("pass firts +\n");
      }
        
      else if (strcmp(ast->u.operation.op, "-") == 0)
        stack = push(stack, (pop(stack) - pop(stack)));
      else if (strcmp(ast->u.operation.op, "/") == 0)
        stack = push(stack, (pop(stack) / pop(stack)));
      else if (strcmp(ast->u.operation.op, "*") == 0)
        stack = push(stack, (pop(stack) * pop(stack)));
      else if (strcmp(ast->u.operation.op, ">") == 0)
        stack = push(stack, (pop(stack) > pop(stack)));
      else if (strcmp(ast->u.operation.op, ">") == 0)
        stack = push(stack, (pop(stack) < pop(stack)));
      else if (strcmp(ast->u.operation.op, ">=") == 0)
        stack = push(stack, (pop(stack) >= pop(stack)));
      else if (strcmp(ast->u.operation.op, "<=") == 0)
        stack = push(stack, (pop(stack) <= pop(stack)));
      else if (strcmp(ast->u.operation.op, "==") == 0)
        stack = push(stack, (pop(stack) == pop(stack)));
      else if (strcmp(ast->u.operation.op, "!=") == 0)
        stack = push(stack, (pop(stack) != pop(stack)));*/

    }else if (strcmp(code->op, "IFTRUE")==0){ //goto

    }else if (strcmp(code, "LOOP")==0){ //goto
        
    }else if (strcmp(code, "IFFALSE")==0){ //goto
        
    }else if (strcmp(code, "SET")==0){
        
    }else if (strcmp(code, "INC")==0){
        
    }else if (strcmp(code, "ENDELSE")==0){ //goto
        
    }else if (strcmp(code, "BASIC")==0){

        printf("executing %s\n", codelist->arg1);
        //sleep(2);
        
    }else if (strcmp(code, "FINISH")==0){

      printf("finish exec\n");
      //creer un nouveau finish et le rajouter dans une pile de l'activité en cours
      program_state->finish_list = add_finish(NULL, finish_counter++);

      exec_code(goto_code(codelist, codelist->label+1), program_state);

    }else if (strcmp(code, "CLOCK_CREATE")==0){

      printf("clock_create exec\n");

      program_state->clock_list = add_clock(program_state->clock_list, codelist->arg1);
      //add activity to clock? how to know which one??
      exec_code(goto_code(codelist, codelist->label+1), program_state);

    }else if (strcmp(code, "END_FINISH")==0){
        
    }else if (strcmp(code, "ASYNC_SUITE")==0){ //goto
      printf("async_suite exec\n");  
      exec_code(goto_code(codelist, codelist->goto_label), program_state);

    }else if (strcmp(code, "EVAL")==0){
        
    }else if (strcmp(code, "CLOCK_REGISTER")==0){
        
    }else if (strcmp(code, "END_ASYNC")==0){
        
    }else if (strcmp(code, "ADVANCE")==0){
      printf("advance exec\n");
        //wait
      advance(find_clock(codelist->arg1, program_state->clock_list), program_state->ready_activities, program_state->activity_list);
      exec_code(goto_code(codelist, codelist->label+1), program_state);
    }else{
        printf("error: code list is indefined\n");
        exit(1);
    }
}
void print_activity(activity* activity){
  struct activity* temp = activity;
  while(temp != NULL){
    printf("A%d \n", temp->id);
    if (temp->finish != NULL) printf("with embracing finish: F%d" , temp->finish->id);
    //printf("\n");
    temp = temp->next;
  }
}

void print_clocks(clocks* clocks){
  struct clocks* temp = clocks;
  while(temp != NULL){
    printf("%s\n", temp->id);
    printf("registered activities: ");
    if (temp->registered != NULL) print_activity(temp->registered);
    printf("blocked activities: ");
    if (temp->blocked != NULL) print_activity(temp->blocked);
    temp = temp->next;
  }
  printf("\n");
}
void print_ready_activity(ready* activity){
  struct ready* temp = activity;
  while(temp != NULL){
    printf("A%d ", temp->activity->id);
    temp = temp->next;
  }
  printf("\n");
}
void print_state(state* state){
  printf("\nPROGRAM STATE:\n\n");
  printf("ready activities: ");
  print_ready_activity(state->ready_activities);
  printf("all program activities: ");
  print_activity(state->activity_list);
  printf("clocks: \n");
  print_clocks(state->clock_list);
}


