%error-verbose
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    #define YYDEBUG 1

    /* Since the parser must return the AST, it must get a parameter where
    * the AST can be stored. The type of the parameter will be void*. */
    #define YYPARSE_PARAM ast_result

    int yylex();
    void yyerror(char *);
    void lex_free();
    extern FILE *yyin;
    extern FILE *yyout;
%}

%union {
  char* string;
  int value;
  struct ast* ast;
  struct clock** clock;
}

%start program
%token WHILE FOR IF ELSE ELSEIF FINISH ASYNC ADVANCE
%token BLANK

%type <ast> affine condition range
%type <ast> stmt block list_stmt
%type <ast> program
%type <clock> clocks
%type <string> instruction

%token <string> COMP RANGE
%token <string> ID
%token <value> NUMBER


%right ID '='
%left '+'
%left '-'
%left '*'
%left '/'
%left COMP


%%
program: list_stmt                                  { printf("rendering AST\n"); (*(ast**)ast_result) = $1; }
    ;
    
stmt:
        instruction ';'                             { printf("INSTRUCTION\n"); $$ = ast_new_id($1); }
    |   FOR ID '=' range block                      { printf("FOR stmt\n"); $$ = ast_new_for($2, $4, $5); }
    |   IF '(' condition ')' block                  { printf("IF stmt\n"); $$ = ast_new_if($3, $5, NULL); }
    |   FINISH '(' clocks ')' block                 { printf("FINISH stmt\n"); $$ = ast_new_parallel("finish", $3, $5); }
    |   ASYNC '(' clocks ')' block                  { printf("ASYNC stmt\n"); $$ = ast_new_parallel("async", $3, $5); }
    |   ADVANCE ID ';'                              { printf("ADVANCE stmt\n"); $$ = ast_new_advance($2); }
    ;

instruction: ID 
    ;

clocks: 
        /*%empty*/                                   { $$ = NULL; }
    |   clocks ',' ID                                {printf("CLOCKS\n"); $$ = push_clock($1, $3);}
    |   ID                                           {printf("FIRST CLOCK\n"); $$ = push_clock(NULL, $1);}
    ;

block:
        '{' list_stmt '}'                           { printf("block\n"); $$ = $2;}
    |   stmt
    ;

range: affine RANGE affine                          { printf("range\n"); $$ = ast_new_range($1, $3); }
    ;

list_stmt:
        list_stmt stmt                              { printf("stmt block\n"); 
                                                      $1->u.statements = new_statements($2, $1->u.statements);
                                                      $$ = $1; 
                                                    }
    |   stmt                                        { printf("stmt\n"); $$ = ast_new_statements(new_statements($1,NULL));}
    ;

affine:
        affine '+' affine                           { $$ = ast_new_operation("+", $1, $3); }
    |   affine '-' affine                           { $$ = ast_new_operation("-", $1, $3); }
    |   affine '/' affine                           { $$ = ast_new_operation("/", $1, $3); }
    |   affine '*' affine                           { $$ = ast_new_operation("*", $1, $3); }
    | '(' affine ')'                                { $$ = $2; }
    | ID                                            { $$ = ast_new_id($1); }
    | NUMBER                                        { $$ = ast_new_number($1); }
    ;

condition:
        affine COMP affine                          { $$ = ast_new_operation($2, $1, $3); }
    ;

%%

int main( int argc, char **argv ) {
    ast *a;

    if( argc != 2 ) {
        printf("Entrez une expression :\n");
    }else{
        yyin = fopen(argv[1], "r");
        yyout = fopen("output.c", "w");
        if (yyout == NULL) {
            fprintf(stderr, "Error creating output file!\n");
            exit(1);
        } else if (!yyin) {
            fprintf(stderr, "Error opening file %s !\n", argv[1]);
            exit(1);
        }
    }

    if (yyparse(&a) == 0) {  
        ast_print(a,0);
    }
    return 0;
}