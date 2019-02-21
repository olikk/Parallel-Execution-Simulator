%error-verbose
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    #define YYDEBUG 1
    //#include "utils.h"
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
  /* Pour les expr bool
  struct {
    quad* code;
    quad_list* truelist;
    quad_list* falselist;
  } code_condition;
  symbol* label;

  %type <code_condition> condition
  %type <label> tag
  */
}

%token WHILE FOR IF ELSE ELSEIF 
%token BLANK

%type <ast> affine

%token <string> COMP
%token <string> ID
%token <value> NUMBER


%right ID '='
%left '+'
%left '-'
%left '*'
%left '/'
%left COMP

//precedence left TYPE ID;

%%

stmt:
        ID ';'
    |   FOR ID "=" NUMBER ".." NUMBER stmt ';'
    |   IF '(' condition ')' stmt ';'
    |   IF '(' condition ')' '{' list_stmt '}' 
    ;

list_stmt: 
    |   list_stmt stmt
    |   stmt
    ;

affine:
        affine "+" affine   { $$ = ast_new_operation("+", $1, $3); }
    |   affine "-" affine   { $$ = ast_new_operation("-", $1, $3); }
    | '(' affine ')'  { $$ = $2; }
    | ID            { $$ = ast_new_id($1); }
    | NUMBER        { $$ = ast_new_number($1); }
    ;
condition:
        affine COMP affine
    ;


%%

int main( int argc, char **argv ) {
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

    return  yyparse();
}