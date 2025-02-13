%error-verbose
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    #include "semantic_check.h"
    #include "codegen.h"
    #include "exec.h"
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
  struct clock* clock;
}

%start program
%token WHILE FOR IF ELSE ELSEIF FINISH ASYNC ADVANCE
%token BLANK

%type <ast> affine range
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
program: list_stmt                                  { printf("rendering AST\n\n"); (*(ast**)ast_result) = $1; }
    ;
    
stmt:
        instruction ';'                             { $$ = ast_new_basic($1); }
    |   FOR ID '=' range block                      { $$ = ast_new_for($2, $4, $5); }
    |   IF '(' affine ')' block                     { $$ = ast_new_if($3, $5, NULL); }
    |   IF '(' affine ')' block ELSE block          { $$ = ast_new_if($3, $5, $7); }
    |   FINISH '(' clocks ')' block                 { $$ = ast_new_parallel("finish", $3, $5); }
    |   FINISH block                                { $$ = ast_new_parallel("finish", NULL , $2); }
    |   ASYNC '(' clocks ')' block                  { $$ = ast_new_parallel("async", $3, $5); }
    |   ASYNC block                                 { $$ = ast_new_parallel("async", NULL, $2); }
    |   ADVANCE ID ';'                              { $$ = ast_new_advance($2); }
    ;

instruction: ID 
    ;

clocks: 
        /*%empty*/                                   { $$ = NULL; }
    |   clocks ',' ID                                {$$ = push_clock($1, $3);}
    |   ID                                           {$$ = push_clock(NULL, $1);}
    ;

block:
        '{' list_stmt '}'                           { $$ = $2;}
    |   stmt
    ;

range: affine RANGE affine                          { $$ = ast_new_range($1, $3); }
    ;

list_stmt:
        list_stmt stmt                              { 
                                                      $1->u.statements = new_statements($2, $1->u.statements);
                                                      $$ = $1; 
                                                    }
    |   stmt                                        { $$ = ast_new_statements(new_statements($1,NULL));}
    ;

affine:
        affine '+' affine                           { $$ = ast_new_operation("+", $1, $3); }
    |   affine '-' affine                           { $$ = ast_new_operation("-", $1, $3); }
    |   affine '/' affine                           { $$ = ast_new_operation("/", $1, $3); }
    |   affine '*' affine                           { $$ = ast_new_operation("*", $1, $3); }
    |   affine COMP affine                          { $$ = ast_new_operation($2, $1, $3); }
    | '(' affine ')'                                { $$ = $2; }
    | ID                                            { $$ = ast_new_id($1); }
    | NUMBER                                        { $$ = ast_new_number($1); }
    ;



%%

int main( int argc, char **argv ) {
    ast *a;

    if( argc < 2 ) {
        printf("Enter instructions :\n");
    }else if (argc == 2){
        yyin = fopen(argv[1], "r");
        yyout = fopen("output.c", "w");
        if (yyout == NULL) {
            fprintf(stderr, "Error creating output file!\n");
            exit(1);
        } else if (!yyin) {
            fprintf(stderr, "Error opening file %s !\n", argv[1]);
            exit(1);
        }
    } else {
        fprintf(stderr, "Too many arguments\n");
        exit(1);
    }

    if (yyparse(&a) == 0) {  
        int i;
        ast_print(a,0);
        printf("\n\n starting semantic check\n\n");
        if ((i = semantic_check(a,NULL)) == 1){
            return 1;
        }
        printf("\n starting code generation\n\n");
        code* codelist = ast_to_code(a, NULL);
        printf("\n code generation done\n");
        if (codelist != NULL){
            printf("\n\n starting code print \n\n");
            code_print(codelist);
            printf("\n end code print\n\n");
            printf("\n starting code execution\n\n");
            run(codelist);
        }else 
            printf("error: empty code list\n");
    } else return 1;
    return 0;
}