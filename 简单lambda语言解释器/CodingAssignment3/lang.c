#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct expr * new_expr_ptr() {
  struct expr * res = (struct expr *) malloc(sizeof(struct expr));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct expr * TConstNat(unsigned int value) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CONST_NAT;
  res -> d.CONST_NAT.value = value;
  return res;
}

struct expr * TVar(char * name) {
  struct expr * res = new_expr_ptr();
  res -> t = T_VAR;
  res -> d.VAR.name = name;
  return res;
}

struct expr * TConstBinOp(enum BinOpType op) {
  struct expr * res = new_expr_ptr();
  res -> t = T_CONST_BINOP;
  res -> d.CONST_BINOP.op = op;
  return res;
}

struct expr * TFunApp(struct expr * left, struct expr * right) {
  struct expr * res = new_expr_ptr();
  res -> t = T_FUN_APP;
  res -> d.FUN_APP.left = left;
  res -> d.FUN_APP.right = right;
  return res;
}

struct expr * TFunAbs(char * name, struct expr * arg) {
  struct expr * res = new_expr_ptr();
  res -> t = T_FUN_ABS;
  res -> d.FUN_ABS.name = name;
  res -> d.FUN_ABS.arg = arg;
  return res;
}

void print_binop(enum BinOpType op) {
  switch (op) {
  case T_PLUS:
    printf("PLUS");
    break;
  case T_MINUS:
    printf("MINUS");
    break;
  case T_MUL:
    printf("MUL");
    break;
  case T_DIV:
    printf("DIV");
    break;
  case T_MOD:
    printf("MOD");
    break;
  }
}

void print_expr(struct expr * e) {
  switch (e -> t) {
  case T_CONST_NAT:
    printf("CONST_NAT(%d)", e -> d.CONST_NAT.value);
    break;
  case T_VAR:
    printf("VAR(%s)", e -> d.VAR.name);
    break;
  case T_CONST_BINOP:
    printf("CONST_BINOP(");
    print_binop(e -> d.CONST_BINOP.op);
    printf(")");
    break;
  case T_FUN_APP:
    printf("FUN_APP(");
    print_expr(e -> d.FUN_APP.left);
    printf(",");
    print_expr(e -> d.FUN_APP.right);
    printf(")");
    break;
  case T_FUN_ABS:
    printf("FUN_ABS(%s,", e -> d.FUN_ABS.name);
    print_expr(e -> d.FUN_ABS.arg);
    printf(")");
    break;
  }
}


