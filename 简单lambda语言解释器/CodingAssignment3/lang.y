%{
	// this part is copied to the beginning of the parser 
	#include <stdio.h>
	#include "lang.h"
	#include "lexer.h"
	void yyerror(char *);
	int yylex(void);
        struct expr * root;
%}

%union {
unsigned int n;
char * i;
struct expr * e;
void * none;
}

// Terminals
%token <n> TM_NAT
%token <i> TM_IDENT
%token <none> TM_LEFT_PAREN TM_RIGHT_PAREN
%token <none> TM_COL
%token <none> TM_LAMBDA
%token <none> TM_PLUS TM_MINUS
%token <none> TM_MUL TM_DIV TM_MOD

// Nonterminals
%type <e> NT_WHOLE
%type <e> NT_EXPR
%type <e> NT_EXPR2
%type <e> NT_EXPR3
%type <e> NT_EXPR4

// Priority
%left TM_PLUS TM_MINUS
%left TM_MUL TM_DIV TM_MOD
%left TM_LEFT_PAREN TM_RIGHT_PAREN

%%

NT_WHOLE:
  NT_EXPR4
  {
    $$ = ($1);
    root = $$;
  }
;

NT_EXPR:
  TM_NAT
  {
    $$ = (TConstNat($1));
  }
| TM_LEFT_PAREN NT_EXPR4 TM_RIGHT_PAREN
  {
    $$ = ($2);
  }
| TM_IDENT
  {
    $$ = (TVar($1));
  }
;

NT_EXPR2:
  NT_EXPR
  {
    $$ = ($1);
  }
| NT_EXPR2 NT_EXPR
  {
    $$ = (TFunApp($1,$2));
  }
;

NT_EXPR3:
  NT_EXPR2
  {
    $$ = ($1);
  }
| NT_EXPR3 TM_PLUS NT_EXPR3
  {
    $$ = (TFunApp(TFunApp(TConstBinOp(T_PLUS),$1),$3));
  }
| NT_EXPR3 TM_MINUS NT_EXPR3
  {
    $$ = (TFunApp(TFunApp(TConstBinOp(T_MINUS),$1),$3));
  }
| NT_EXPR3 TM_MUL NT_EXPR3
  {
    $$ = (TFunApp(TFunApp(TConstBinOp(T_MUL),$1),$3));
  }
| NT_EXPR3 TM_DIV NT_EXPR3
  {
    $$ = (TFunApp(TFunApp(TConstBinOp(T_DIV),$1),$3));
  }
| NT_EXPR3 TM_MOD NT_EXPR3
  {
    $$ = (TFunApp(TFunApp(TConstBinOp(T_MOD),$1),$3));
  }
;

NT_EXPR4:
  NT_EXPR3
  {
    $$ = ($1);
  }
| TM_LAMBDA TM_IDENT TM_COL NT_EXPR4
  {
    $$ = (TFunAbs($2,$4));
  }
;

%%

void yyerror(char* s)
{
    fprintf(stderr , "%s\n",s);
}
