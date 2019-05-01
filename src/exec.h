#ifndef EXEC_H
#define EXEC_H

typedef struct activity{
    int id;
    int program_counter;
    struct finish* finish_stack; //pile
    //eval stack 
    struct pile* stack;
    struct clocks* clock_registered;
    struct activity* next;
} activity;

typedef struct clocks {
    char* id;
    activity* registered;
    activity* blocked;
    struct clocks* next;
} clocks;

typedef struct finish{
    int id;
    clocks* clocks;
    activity* activities;
    int wait;
    struct finish* next;
} finish;

typedef struct ready{
    activity* activity;
    struct ready* next;
} ready;

typedef struct state{

    //a pile of activities ready to be executed
    ready* ready_activities;

    ready* running;

} state;

ready* add_ready(ready* ready, activity* a);
void print_state(state* state);
activity* add_activity(activity* root, int id, finish* f);
void run(code*, state*);

#endif