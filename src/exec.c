#include "codegen.h"
#include "exec.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "pile.h"

int activity_counter = 0;
int finish_counter = 0;


clocks* create_clock(char* id){
	clocks* new = check_alloc(sizeof(struct clocks));
	new->registered = NULL;
	new->blocked = NULL;
	new->name = id;
    new->next = NULL;
	return new;
}
clocks* add_clock(clocks* clock_list, clocks* new){
	new->next = clock_list; 
	return new;
}
clocks_list* create_clock_list(clocks* c){
	clocks_list* new = check_alloc(sizeof(struct clocks_list));
	new->element = c;
    new->next = NULL;
	return new;
}
clocks_list* add_clock_list(clocks_list* clock_list, clocks_list* new){
	new->next = clock_list; 
	return new;
}
clocks* find_clock(char* id, clocks_list* list){
	struct clocks_list* temp = list;
	while(temp != NULL){
		if (strcmp(temp->element->name, id) == 0){
            break;
        } else{
		    temp = temp->next;
        }
	}
	return temp->element;
}

void remove_activity(activity_list** list, int id){
    struct activity_list* curr; //clk->registered;
    /* For 1st node, indicate there is no previous. */
    struct activity_list* prev = NULL;
    /*
    * Visit each node, maintaining a pointer to
    * the previous node we just visited.
    */
    for (curr = *list; curr != NULL; prev = curr, curr = curr->next) {
        if (curr->element->id == id) {  /* Found it. */
            if (prev == NULL) {
                /* Fix beginning pointer. */
                *list = curr->next;
            } else {
                /*
                * Fix previous node's next to
                * skip over the removed node.
                */
                prev->next = curr->next;
            } 
            /* Deallocate the node. */
            free(curr);
            /* Done searching. */
            return;
        }
    }
}

activity* create_activity(int id){
	activity* new = check_alloc(sizeof(struct activity));
	new->id = id;
	new->next = NULL;
    new->finish_stack = NULL;
    new->registered_with = NULL;
	return new;
}
activity* find_activity(int id, activity_list* act){
	struct activity_list* temp = act;
	while (temp != NULL){
		if (temp->element->id == id){
            return temp->element;
        }	
        temp = temp->next;
	}
	return NULL;
}

activity* add_activity(activity* root, activity* new){
	activity* tmp = root;
	if (root == NULL){
	    root = new;
    }else{
		while(tmp->next != NULL){
            tmp = tmp->next;
        }	
		tmp->next = new;
        
	}
	return root;
}
activity_list* create_activity_list(activity* a){
	activity_list* new = check_alloc(sizeof(struct activity_list));
	new->element = a;
	new->next = NULL;
	return new;
}
activity_list* add_activity_list(activity_list* root, activity_list* new){
	activity_list* tmp = root;
	if (root == NULL){
	    root = new;
    }else{
		while(tmp->next != NULL){
            tmp = tmp->next;
        }	
		tmp->next = new;
        
	}
	return root;
}
int activity_list_length(activity_list* act){
	int len = 0;
	struct activity_list* temp = act;
	while (temp != NULL){
		len++;
        temp = temp->next;
	}
	return len;
}
int compare_activities(activity_list* list1, activity_list* list2){
	activity_list* a = list1;
    if (a == NULL && list2 == NULL){
        return 1;
    }
	if ( activity_list_length(list1) != activity_list_length(list2)) {
		return 0;
	}
	while (a != NULL) { 
		if (find_activity(a->element->id, list2) == NULL){
            return 0;
        }
        a = a->next;
    }
    
	return 1; 
}

finish* create_finish(int id){
	finish* new = check_alloc(sizeof(struct finish));
	new->id = id;
    new->activities = NULL;
    new->clocks = NULL;
	new->next = NULL;
	return new;
}

finish* add_finish(finish* root, finish* new){
	new->next = root;
	return new;
}
finish_list* create_finish_list(finish* f){
	finish_list* new = check_alloc(sizeof(struct finish_list));
	new->element = f;
	new->next = NULL;
	return new;
}

finish_list* add_finish_list(finish_list* root, finish_list* new){
	new->next = root;
	return new;
}
activity_list* remove_ready_activity(activity_list* act){
	return act->next;
}
void clock_free(clocks* clock){
	free(clock->registered);
	free(clock->blocked);
	free(clock);
}

 

void run(code* codelist){
    program_state = check_alloc(sizeof(struct state));
	activity_list* current = create_activity_list(create_activity(activity_counter++));
    program_state->ready_activities = add_activity_list(NULL, current);
	program_state->running = current;
	while (program_state->ready_activities != NULL){
		execute(program_state->running, codelist);
	}
}

void execute(activity_list* act, code* codelist){
	code* code = goto_code(codelist, act->element->program_counter);
	act->element->program_counter = act->element->program_counter + 1; //next instruction

	if (strcmp(code->op, "PUSH")==0) {

		printf("push oper\n");

	}else if (strcmp(code->op, "IFTRUE")==0){ //goto

		act->element->program_counter = code->goto_label;

	}else if (strcmp(code->op, "LOOP")==0){ //goto

		act->element->program_counter = code->goto_label;

	}else if (strcmp(code->op, "IFZERO")==0){ //goto

		act->element->program_counter = code->goto_label; 

	}else if (strcmp(code->op, "SET")==0){
				
	}else if (strcmp(code->op, "INC")==0){
				
	}else if (strcmp(code->op, "ENDELSE")==0){ //goto

		act->element->program_counter = code->goto_label;

	}else if (strcmp(code->op, "BASIC")==0){

		printf("executing %s\n", code->arg1);

	}else if (strcmp(code->op, "FINISH")==0){

		printf("finish exec\n");
		//creer un nouveau finish et le rajouter dans une pile de l'activité en cours
		finish* new = create_finish(finish_counter++);
		new->parent = program_state->running->element;
		act->element->finish_stack= add_finish_list(act->element->finish_stack, create_finish_list(new));

	}else if (strcmp(code->op, "CLOCK_CREATE")==0){

		printf("clock_create exec\n");
		//create clock and add it to current activity
		act->element->registered_with = add_clock_list(act->element->registered_with , create_clock_list(create_clock(code->arg1)));
		printf("created: %s\n ",act->element->registered_with->element->name);
		//add current activity to this clock
		act->element->registered_with->element->registered = add_activity_list(act->element->registered_with->element->registered, create_activity_list(act->element));
		//add clock to finish
		act->element->finish_stack->element->clocks = add_clock_list(act->element->finish_stack->element->clocks, create_clock_list(act->element->registered_with->element));

	}else if (strcmp(code->op, "END_PREAMBLE")==0){ 
		printf("end_preamble\n");
        if ((code->arg1 != NULL) && (strcmp(code->arg1, "GOTO")==0)){ //goto (async)

			//change to the main activity after register clocks
			program_state->running = program_state->ready_activities; 
			program_state->running->element->program_counter = code->goto_label;
		}
	}else if (strcmp(code->op, "END_FINISH")==0){
		
        // if activities file inside this finish is not empty
        if (act->element->finish_stack->element->activities != NULL){
            act->element->finish_stack->element->is_waiting = 1;
            //for each clock of this finish remove current activity from their pile 'registered'
            clocks_list* temp = act->element->finish_stack->element->clocks;
			
            while( temp != NULL){
                remove_activity(&(temp->element->registered), act->element->id);
                temp = temp->next;
            }
            //remove current activity from ready
            program_state->ready_activities = remove_ready_activity(program_state->ready_activities );
			program_state->running = program_state->ready_activities;
        } else { 
			//if all activities are done
            act->element->finish_stack->element->is_waiting = 0;

            //destroy each clock of this finish 
            clocks_list* temp = act->element->finish_stack->element->clocks;

            while( act->element->finish_stack->element->clocks != NULL) {

                if(act->element->finish_stack->element->clocks->element->registered != NULL) free(act->element->finish_stack->element->clocks->element->registered);
                if(act->element->finish_stack->element->clocks->element->blocked != NULL) free(act->element->finish_stack->element->clocks->element->blocked);

                temp = act->element->finish_stack->element->clocks->next;
                free(act->element->finish_stack->element->clocks);

                act->element->finish_stack->element->clocks = temp;
            }
			//si l'activité qui a créé le finish est bloquée -> le mettre en ready
			if (find_activity(act->element->finish_stack->element->parent->id, program_state->ready_activities) == NULL){
				program_state->ready_activities = add_activity_list(program_state->ready_activities, create_activity_list(act->element->finish_stack->element->parent));
				program_state->running = program_state->ready_activities;
			}
        }
		printf("end finish\n");

	}else if (strcmp(code->op, "ASYNC")==0){ 

		printf("async exec\n"); 
        //create directly new activity 
        activity* new = create_activity(activity_counter++);
        new->program_counter = act->element->program_counter;
        //add embracing finish to this new activity finish stack
        new->finish_stack = add_finish_list(new->finish_stack, create_finish_list(act->element->finish_stack->element));
        //add it to ready activities
		activity_list* elmt  = create_activity_list(new);
        act = add_activity_list(act, elmt);
        //add it to finish created activities
        act->element->finish_stack->element->activities = add_activity_list(act->element->finish_stack->element->activities, elmt);
		//change to other activities
		program_state->running = program_state->running->next;

	}else if (strcmp(code->op, "CLOCK_REGISTER")==0){ 

		printf("clock register\n");
        clocks* clk = find_clock(code->arg1, act->element->finish_stack->element->clocks);
        //add clock to activity clock list
		act->element->registered_with = add_clock_list(act->element->registered_with, create_clock_list(clk));
        //add curent activity to clock registered list
        clk->registered = add_activity_list(clk->registered, create_activity_list(act->element));

	}else if (strcmp(code->op, "END_ASYNC")==0){
		print_ready_activities(act);
		//remove current activity from registered activities pile for each clock that it's registered with it
        clocks_list* clk = act->element->registered_with;
        while (clk != NULL){
            remove_activity(&(clk->element->registered),act->element->id);
			//le test entre blocked et registered
			if (compare_activities(clk->element->blocked, clk->element->registered) == 1){    //if identical
				//copy activities from blocked list to ready activities and empty blocked list
				activity_list* temp = clk->element->blocked;
				while (clk->element->blocked != NULL){
					program_state->ready_activities = add_activity_list(program_state->ready_activities, create_activity_list(clk->element->blocked->element));
					temp = clk->element->blocked->next;
					free(clk->element->blocked);
					clk->element->blocked = temp;
				}
			}
			clk = clk->next;
        }
        //remove current activity from finish 
        remove_activity(&(act->element->finish_stack->element->activities), act->element->id);
        //remove from ready
        program_state->ready_activities = remove_ready_activity(program_state->ready_activities);
		program_state->running = program_state->ready_activities;
		program_state->running->element->program_counter = act->element->program_counter;

	}else if (strcmp(code->op, "ADVANCE")==0){
		print_ready_activities(act);
		printf("advance exec\n");
		//add current activity to clock blocked pile
        clocks* clk = find_clock(code->arg1, act->element->registered_with);
		if (find_activity(act->element->id,clk->blocked ) == NULL){
        	clk->blocked = add_activity_list(clk->blocked, create_activity_list(act->element));
			//remove form ready
	    	program_state->ready_activities = remove_ready_activity(program_state->ready_activities);
			program_state->running = program_state->ready_activities;
		}
        // compare blocked and registered
        if (compare_activities(clk->blocked, clk->registered) == 1){    //if identical
            //copy activities from blocked list to ready activities and empty blocked list
            activity_list* temp = clk->blocked;
            while (clk->blocked != NULL){
                program_state->ready_activities = add_activity_list(program_state->ready_activities, create_activity_list(clk->blocked->element));
                temp = clk->blocked->next;
                free(clk->blocked);
                clk->blocked = temp;
            }
			program_state->running = program_state->ready_activities;
        }
        printf("end advance\n");

	}else{

		printf("error: code list is undefined\n");
		exit(1);
	}
}

//////////////////////////////  PRINT FUNCTIONS   //////////////////////////////////////

void print_clocks_name(clocks_list* clocks){
	struct clocks_list* temp = clocks;
	while(temp != NULL){
		printf("%s ", temp->element->name);
        temp = temp->next;
	}
}
void print_activities_name(activity_list* activity){
	struct activity_list* temp = activity;
	while(temp != NULL){
		printf("A%d  ", temp->element->id);
        temp = temp->next;
	}
}
void print_finish(finish_list* f){
	struct finish_list* temp = f;
	while(temp != NULL){
		printf("F%d\n", temp->element->id);
		printf("created activities: ");
		print_activities_name(temp->element->activities);
		printf("created clocks: ");
		print_clocks_name(temp->element->clocks);
		temp = temp->next;
		printf("\n		");
	}
	printf("\n");
}

void print_clocks(clocks_list* clocks){
	struct clocks_list* temp = clocks;
	while(temp != NULL){
		printf("%s:\n", temp->element->name);
		printf("registered activities: ");
		if (temp->element->registered != NULL) print_activities_name(temp->element->registered);
		printf("blocked activities: ");
		if (temp->element->blocked != NULL) print_activities_name(temp->element->blocked);
		temp = temp->next;
		printf("\n		");
	}
	printf("\n");
}

void print_ready_activities(activity_list* activity){
	struct activity_list* temp = activity;
	while(temp != NULL){
		printf("A%d: \n    ", temp->element->id);
		printf("finish pile:  ");
		if (temp->element->finish_stack != NULL) print_finish(temp->element->finish_stack);
		if (temp->element->registered_with != NULL){
            printf("\n     clocks:  ");
            print_clocks(temp->element->registered_with);
        } 
		temp = temp->next;
		printf("\n");
	}
	printf("\n\n");
}

void print_state(state* state){
	printf("\nPROGRAM STATE:\n\n");
	printf("ready activities: ");
	print_ready_activities(state->ready_activities);
}


