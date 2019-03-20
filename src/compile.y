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
}

%start program
%token WHILE FOR IF ELSE ELSEIF FINISH ASYNC ADVANCE
%token BLANK

%type <ast> affine condition range
%type <ast> stmt list_stmt block
%type <ast> program
%type <string> clocks

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
program: list_stmt                                  { printf("rendering AST\n"); ast_print($1, 0); (*(ast**)ast_result) = $1; }
    ;
stmt:
        ID ';'                                      { printf("ID stmt\n"); $$ = ast_new_id($1); }
    |   FOR ID '=' range block                      { printf("FOR stmt\n"); $$ = ast_new_for($2, $4, $5); }
    |   IF '(' condition ')' block                  { printf("IF stmt\n"); $$ = ast_new_if($3, $5, NULL); }
    |   FINISH '(' clocks ')' block                         { printf("FINISH stmt\n"); $$ = ast_new_parallel("finish", $3, $5); }
    |   ASYNC '(' clocks ')' block                         { printf("ASSYNC stmt\n"); $$ = ast_new_parallel("async", $3, $5); }
    ;

clocks: 
        /*%empty*/          //creer une liste vide ?
    |   ID ',' clocks                                {printf("CLOCKS\n"); $$ = new_clocks($3, $1);}
    |   ID
    ;

block:
        '{' list_stmt '}'                           { printf("block\n"); $$ = ast_new_statements(NULL, $2); }
    |   stmt
    ;

range: affine RANGE affine                          { printf("range\n"); $$ = ast_new_range($1, $3); }
    ;

list_stmt: 
         /*%empty*/  
    |   list_stmt stmt                              { printf("stmt block\n"); $$ = ast_new_statements($1, $2); }
    |   stmt
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
    return  yyparse(&a);
}