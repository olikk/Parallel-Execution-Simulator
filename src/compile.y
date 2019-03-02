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
%start program
%token WHILE FOR IF ELSE ELSEIF 
%token BLANK

%type <ast> affine condition range
%type <ast> stmt list_stmt
%type <ast> program

%token <string> COMP RANGE
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
program: list_stmt                                  { printf("rendering AST\n"); ast_print($1, 0); }
    ;
stmt:
        ID ';'                                      { printf("ID stmt\n"); $$ = ast_new_id($1); }
    |   FOR ID '=' range stmt                    { printf("FOR stmt\n"); $$ = ast_new_for($2, $4, $5); }
    |   IF '(' condition ')' stmt                   { printf("IF stmt\n"); $$ = ast_new_if($3, $5, NULL); }
    |   IF '(' condition ')' '{' list_stmt '}'      { $$ = ast_new_if($3, $6, NULL); }
    ;

range: NUMBER RANGE NUMBER                           { printf("range\n");$$ = ast_new_operation("range", $1, $3); }
    ;

list_stmt: 
    |   list_stmt stmt
    |   stmt
    ;



affine:
        affine '+' affine                          { $$ = ast_new_operation("+", $1, $3); }
    |   affine '-' affine                           { $$ = ast_new_operation("-", $1, $3); }
    | '(' affine ')'                                { $$ = $2; }
    | ID                                            { $$ = ast_new_id($1); }
    | NUMBER                                        { $$ = ast_new_number($1); }
    ;

condition:
        affine COMP affine                          { $$ = ast_new_operation($2, $1, $3); }
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