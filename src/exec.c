#include "code.h"
#include "exec.h"
#include <stdlib.h>
#include <stdio.h>

state* exec_code(code* codelist){
    if (strcmp(codelist->op, 'EVAL')==0) {

    }else if (strcmp(codelist->op, 'IFTRUE')==0){

    }else if (strcmp(codelist->op, 'LOOP')==0){
        
    }else if (strcmp(codelist->op, 'IFFALSE')==0){
        
    }else if (strcmp(codelist->op, 'SET')==0){
        
    }else if (strcmp(codelist->op, 'INC')==0){
        
    }else if (strcmp(codelist->op, 'ENDELSE')==0){
        
    }else if (strcmp(codelist->op, 'BASIC')==0){
        
    }else if (strcmp(codelist->op, 'FINISH')==0){
        
    }else if (strcmp(codelist->op, 'CLOCK_CREATE')==0){
        
    }else if (strcmp(codelist->op, 'END_FINISH')==0){
        
    }else if (strcmp(codelist->op, 'ASYNC_SUITE')==0){
        
    }else if (strcmp(codelist->op, 'EVAL')==0){
        
    }else if (strcmp(codelist->op, 'CLOCK_REGISTER')==0){
        
    }else if (strcmp(codelist->op, 'END_ASYNC')==0){
        
    }else if (strcmp(codelist->op, 'ADVANCE')==0){
        
    }
    
  return state;
}


