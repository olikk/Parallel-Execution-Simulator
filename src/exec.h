#ifndef EXEC_H
#define EXEC_H

struct state* program_state;

typedef struct activity{
    int id;
    int program_counter;
    struct finish_list* finish_stack; //pile
    //eval stack 
    struct pile* stack;
    struct clocks_list* registered_with;
    struct activity* next;
} activity;

typedef struct activity_list{
    activity* element;
    struct activity_list* next;
} activity_list;

typedef struct clocks {
    char* name;
    activity_list* registered;
    activity_list* blocked;
    struct clocks* next;
} clocks;

typedef struct clocks_list{
    clocks* element;
    struct clocks_list* next;
} clocks_list;

typedef struct finish{
    int id;
    clocks_list* clocks;
    activity_list* activities;
    activity* parent;  
    int is_waiting;
    struct finish* next;
} finish;

typedef struct finish_list{
    finish* element;
    struct finish_list* next;
} finish_list;

typedef struct state{
    //a pile of activities ready to be executed
    activity_list* ready_activities;
    activity_list* running;
} state;

void print_clocks_name(clocks_list* clocks);
void print_activities_name(activity_list* activity);
void print_ready_activities(activity_list*);
void run(code*);
void execute(activity_list* act, code* codelist);
#endif