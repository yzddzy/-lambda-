#ifndef CLOSURE_H_INCLUDED
#define CLOSURE_H_INCLUDED

#include "lang.h"

struct value;

struct var_val_list {
  char * var;
  struct value * val;
  struct var_val_list * link;
};

enum ValueType {
  TV_NAT,
  TV_BINOP,
  TV_BINOP_NAT,
  TV_CLOS
};

struct value {
  enum ValueType t;
  union {
    struct {unsigned int n; } NAT;
    struct {enum BinOpType op; } BINOP;
    struct {enum BinOpType op; unsigned int n; } BINOP_NAT;
    struct {struct expr * e; struct var_val_list * l; } CLOS;
  } d;
};

void print_value(struct value * v);
struct value * V_Nat(unsigned int n);
struct value * V_Binop(enum BinOpType op);
struct value * V_BinopNat(enum BinOpType op, unsigned int n);
struct value * V_Clos(struct expr * e, struct var_val_list * l);
struct var_val_list * VVL_Nil();
struct var_val_list * VVL_Cons(char * var, struct value * val, struct var_val_list * l);

#endif
