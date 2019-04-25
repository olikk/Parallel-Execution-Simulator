#include "code.h"
#include "exec.h"
#include <stdlib.h>
#include <stdio.h>

int activity = 0;
int finish = 0;
state* exec_code(code* codelist){

    //pile avec les activité pour assync

    if (strcmp(codelist->arg1, 'GOTO')==0) {
        exec_code(goto_code(codelist, codelist->goto_label));
    }
    if (strcmp(codelist->op, 'EVAL')==0) {

    }else if (strcmp(codelist->op, 'IFTRUE')==0){ //goto

    }else if (strcmp(codelist->op, 'LOOP')==0){ //goto
        
    }else if (strcmp(codelist->op, 'IFFALSE')==0){ //goto
        
    }else if (strcmp(codelist->op, 'SET')==0){
        
    }else if (strcmp(codelist->op, 'INC')==0){
        
    }else if (strcmp(codelist->op, 'ENDELSE')==0){ //goto
        
    }else if (strcmp(codelist->op, 'BASIC')==0){

        printf("executing %s\n", codelist->arg1);
        sleep(2);
        
    }else if (strcmp(codelist->op, 'FINISH')==0){
        
    }else if (strcmp(codelist->op, 'CLOCK_CREATE')==0){
        //une liste des activités enregistre avec ce clock
    }else if (strcmp(codelist->op, 'END_FINISH')==0){
        
    }else if (strcmp(codelist->op, 'ASYNC_SUITE')==0){ //goto
        
    }else if (strcmp(codelist->op, 'EVAL')==0){
        
    }else if (strcmp(codelist->op, 'CLOCK_REGISTER')==0){
        
    }else if (strcmp(codelist->op, 'END_ASYNC')==0){
        
    }else if (strcmp(codelist->op, 'ADVANCE')==0){
        //wait
    }else{
        printf("error: code list is indefined\n");
        exit(1);
    }
    
  return state;
}


