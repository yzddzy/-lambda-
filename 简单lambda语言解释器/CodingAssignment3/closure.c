#include "lang.h"
#include "closure.h"

struct value * new_value_ptr() {
  struct value * res = (struct value *) malloc(sizeof(struct value));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct var_val_list * new_var_val_list_ptr() {
  struct var_val_list * res = (struct var_val_list *) malloc(sizeof(struct var_val_list));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

void print_var_val_list(struct var_val_list * l, unsigned int number_of_space);

void print_value_rec(struct value * v, unsigned int number_of_space) {
  switch (v -> t) {
  case TV_NAT:
    printf("%d\n", v -> d.NAT.n);
    break;
  case TV_BINOP:
    print_binop(v -> d.BINOP.op);
    printf("\n");
    break;
  case TV_BINOP_NAT:
    printf("%d ", v -> d.BINOP_NAT.n);
    print_binop(v -> d.BINOP_NAT.op);
    printf("\n");
    break;
  case TV_CLOS:
    print_expr(v -> d.CLOS.e);
    printf("\n");
    print_var_val_list(v -> d.CLOS.l, number_of_space + 2);
    break;
  }
}

void print_var_val_list(struct var_val_list * l, unsigned int number_of_space) {
  int i;
  while (l) {
    for (i = 0; i < number_of_space; ++ i) {
      printf(" ");
    }
    printf("%s := ", l -> var);
    print_value_rec(l -> val, number_of_space);
    l = l -> link;
  }
}

void print_value(struct value * v) {
  print_value_rec(v, 0);
}

struct value * V_Nat(unsigned int n) {
  struct value * res = new_value_ptr();
  res -> t = TV_NAT;
  res -> d.NAT.n = n;
  return res;
}

struct value * V_Binop(enum BinOpType op) {
  struct value * res = new_value_ptr();
  res -> t = TV_BINOP;
  res -> d.BINOP.op = op;
  return res;
}

struct value * V_BinopNat(enum BinOpType op, unsigned int n) {
  struct value * res = new_value_ptr();
  res -> t = TV_BINOP_NAT;
  res -> d.BINOP_NAT.op = op;
  res -> d.BINOP_NAT.n = n;
  return res;
}

struct value * V_Clos(struct expr * e, struct var_val_list * l) {
  struct value * res = new_value_ptr();
  res -> t = TV_CLOS;
  res -> d.CLOS.e = e;
  res -> d.CLOS.l = l;
  return res;
}

struct var_val_list * VVL_Nil() {
  return NULL;
}

struct var_val_list * VVL_Cons(char * var, struct value * val, struct var_val_list * l) {
  struct var_val_list * res = new_var_val_list_ptr();
  res -> var = var;
  res -> val = val;
  res -> link = l;
  return res;
}

