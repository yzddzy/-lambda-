#include <stdlib.h>
#include <string.h>
#include "lang.h"
#include "closure.h"
#include "interpreter.h"

enum ContType {
  T_APP_L,
  T_APP_R,
  T_CALL
};

struct cont_list {
  enum ContType t;
  union {
    struct {struct expr * r; } APP_L;
    struct {struct value * l; } APP_R;
    struct {struct var_val_list * vvl; } CALL; // CALL表示退出当前环境后，要把rho置为vvl
  } d;
  struct cont_list * link;
};

enum ResProgType {
  T_VAL,
  T_FOC
};

struct res_prog {
  enum ResProgType t;
  union {
    struct {struct expr * foc; } FOC; // focused expression 情况
    struct {struct value * val; } VAL; // value 情况
  } d;
  struct cont_list * ectx;
};

struct var_val_list * rho = NULL;

struct res_prog * new_res_prog_ptr() {
  struct res_prog * res = (struct res_prog *) malloc(sizeof(struct res_prog));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cont_list * new_cont_list_ptr() {
  struct cont_list * res = (struct cont_list *) malloc(sizeof(struct cont_list));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct cont_list * CL_Nil() {
  return NULL;
}

struct cont_list * CL_Cons_Call(struct var_val_list * vvl, struct cont_list * l) {
  struct cont_list * res = new_cont_list_ptr();
  res -> t = T_CALL;
  res -> d.CALL.vvl = vvl;
  res -> link = l;
  return res;
}

struct cont_list * CL_Cons_AppL(struct expr * r, struct cont_list * link) {
  struct cont_list * res = new_cont_list_ptr();
  res -> t = T_APP_L;
  res -> d.APP_L.r = r;
  res -> link = link;
  return res;
}

struct cont_list * CL_Cons_AppR(struct value * l, struct cont_list * link) {
  struct cont_list * res = new_cont_list_ptr();
  res -> t = T_APP_R;
  res -> d.APP_R.l = l;
  res -> link = link;
  return res;
}

// 在rho中寻找变量的值
struct value * get_val(char * name) {
  struct var_val_list * l = rho;
  while (l) {
    if (strcmp(name, l -> var) == 0) {
      return l -> val;
    }
    l = l -> link;
  }
  printf("Failure in get_val.\n");
  exit(0);
}

struct res_prog * init_res_prog(struct expr * e) {
  struct res_prog * res = new_res_prog_ptr();
  res -> t = T_FOC;
  res -> d.FOC.foc = e;
  res -> ectx = CL_Nil();
  return res;
}

void step(struct res_prog * r) {
  if (r -> t == T_VAL) {
    if (r -> ectx == NULL) {
      return;
      // impossible
    }
    else {
      struct value * v = r -> d.VAL.val;
      struct cont_list * cl = r -> ectx;
      switch (cl -> t) {
      case T_CALL:
        // TODO: 请实现相关代码
        rho = cl->d.CALL.vvl;
        r->ectx = r->ectx->link;
        break;
      case T_APP_L:
        // TODO: 请实现相关代码
        r->ectx = CL_Cons_AppR(v, cl->link);
        r->t = T_FOC;
        r->d.FOC.foc = cl->d.APP_L.r;
        break;
      case T_APP_R:
        switch (cl -> d.APP_R.l -> t) {
        case TV_NAT:
          printf("Failure! Int as function.\n");
          exit(0);
        case TV_BINOP:
          if (v -> t == TV_NAT) {
            // TODO: 请实现相关代码
            r->d.VAL.val = V_BinopNat(cl->d.APP_R.l->d.BINOP.op, v->d.NAT.n);
            r->ectx = r->ectx->link;
            break;
          }
          else {
            printf("Failure! An arith operator applied on Non-Integer.\n");
            exit(0);
          }
        case TV_BINOP_NAT:
          if (v -> t == TV_NAT) {
            // TODO: 请实现相关代码
            unsigned int a;
            switch (cl->d.APP_R.l->d.BINOP_NAT.op) {
              case T_PLUS:
                a = v->d.NAT.n + cl->d.APP_R.l->d.BINOP_NAT.n;
                break;
              case T_MINUS:
                a = cl->d.APP_R.l->d.BINOP_NAT.n - v->d.NAT.n;
                break;
              case T_MUL:
                a = cl->d.APP_R.l->d.BINOP_NAT.n * v->d.NAT.n;
                break;
              case T_DIV:
                a = cl->d.APP_R.l->d.BINOP_NAT.n / v->d.NAT.n;
                break;
              case T_MOD:
                a = cl->d.APP_R.l->d.BINOP_NAT.n % v->d.NAT.n;
                break;
              default:
                printf("Failure!");
                exit(0);
                break;
            }
            r->d.VAL.val = V_Nat(a);
            r->ectx = r->ectx->link;
            break;
          }
          else {
            printf("Failure! An arith operator applied on Non-Integer as second arg.\n");
            exit(0);
          }
        case TV_CLOS:
          if (cl -> d.APP_R.l -> d.CLOS.e -> t == T_FUN_ABS) {
            if (cl -> d.APP_R.l -> d.CLOS.e -> d.FUN_ABS.arg -> t == T_FUN_ABS) {
              // TODO: 请实现相关代码（闭包中填上一个变量的值后，未来还需继续填变量的值，才能对闭包内的函数体进行运算求值）
              struct var_val_list * vvln = VVL_Cons(cl->d.APP_R.l->d.CLOS.e->d.FUN_ABS.name, r->d.VAL.val, cl->d.APP_R.l->d.CLOS.l);
              r->t = T_VAL;
              r->d.VAL.val = V_Clos(cl->d.APP_R.l->d.CLOS.e->d.FUN_ABS.arg, vvln);
              r->ectx = r->ectx->link;
            }
            else {
              // TODO: 请实现相关代码（闭包中填上一个变量的值后，已经可以对闭包内的函数体进行运算求值，注意此时要切换rho的环境）
              r->ectx = CL_Cons_Call(rho, r->ectx->link);
              rho = VVL_Cons(cl->d.APP_R.l->d.CLOS.e->d.FUN_ABS.name, r->d.VAL.val, cl->d.APP_R.l->d.CLOS.l);
              r->t = T_FOC;
              r->d.FOC.foc = cl->d.APP_R.l->d.CLOS.e->d.FUN_ABS.arg;
            }
            break;
          }
          else {
            printf("Failure! Ilegal Closure appears.\n");
            exit(0);
          }
        }
      }
    }
  }
  else {
    struct expr * e = r -> d.FOC.foc;
    switch (e -> t) {
    case T_CONST_NAT:
      // TODO: 请实现相关代码
      r->t = T_VAL;
      r->d.VAL.val = V_Nat(e->d.CONST_NAT.value);
      break;
    case T_CONST_BINOP:
      // TODO: 请实现相关代码
      r->t = T_VAL;
      r->d.VAL.val = V_Binop(e->d.CONST_BINOP.op);
      break;
    case T_VAR:
      // TODO: 请实现相关代码
      r->t = T_VAL;
      r->d.VAL.val = get_val(e->d.VAR.name);
      break;
    case T_FUN_APP:
      // TODO: 请实现相关代码
      r->t = T_FOC;
      r->d.FOC.foc = e->d.FUN_APP.left;
      r->ectx = CL_Cons_AppL(e->d.FUN_APP.right, r->ectx);
      break;
    case T_FUN_ABS:
      // TODO: 请实现相关代码
      r->t = T_VAL;
      r->d.VAL.val = V_Clos(e, rho);
      break;
    }
  }
}

int test_end(struct res_prog * r) {
  if (r -> t == T_VAL && r -> ectx == NULL) {
    if (r -> d.VAL.val -> t == TV_NAT) {
      printf("%d\n", r -> d.VAL.val -> d.NAT.n);
    }
    else {
      printf("Non-integer value.\n");
    }
    return 1;
  }
  else {
    return 0;
  }
}
