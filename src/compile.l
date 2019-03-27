%{
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
	
%}
IDENTIFIER							[a-zA-Z_]+[0-9a-zA-Z_]*
NUMBER 								[0-9]*
%option yylineno

%%


[()=+\-*,/\[\]{;}]				{ return yytext[0]; } 			

"finish"						{yylval.string = strdup(yytext);
									return FINISH;}

"async"						{yylval.string = strdup(yytext);
									return ASYNC;}	

"advance"						{yylval.string = strdup(yytext);
									return ADVANCE;}																

"if"						{yylval.string = strdup(yytext);
									return IF;}

"else"						{yylval.string = strdup(yytext);
									return ELSE;}

"else if"					{yylval.string = strdup(yytext);
									return ELSEIF;}

"while"						{yylval.string = strdup(yytext);
									return WHILE;}

"for"						{yylval.string = strdup(yytext);
									return FOR;}

{IDENTIFIER}				{yylval.string = strdup(yytext);
									return ID;}	

{NUMBER}					{yylval.value=atoi(yytext);
									return NUMBER;}

">"|"<"|"=<"|">="|"=="|"!="		{yylval.string = strdup(yytext);
									return COMP;
									}					

".."						{yylval.value=atoi(yytext);
									return RANGE;}
"&&"						{}

"||"						{}

[\\n]						{return yytext[0];
									return BLANK;}
\t|\r|[ ]  					{}
.							{printf("[Lex] Unknown character: %s\n", yytext);}
%%
// Free the memory allocated for Lex when we are done.
void lex_free() {
  yy_delete_buffer(YY_CURRENT_BUFFER);
  free(yy_buffer_stack);
}

