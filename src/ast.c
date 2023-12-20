/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#include "ast.h"
#include "evaluate.h"
#include "token.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const *ast_op_type_to_str[] = {
   "+", 
   "-",
   "*",
   "/",
   "%",
   "**",
   "(unknown)",
};

i32_t indent = 0;
char *ast_print_buf = NULL;
i32_t a_blen = 0;
i32_t a_bcap = 1;

static void ast_print_buf_resize(i32_t nlen) {
#define AST_PRINT_BUF_GROW_FACTOR 2
    if (nlen >= a_bcap) {
        a_bcap *= AST_PRINT_BUF_GROW_FACTOR;
        a_bcap = (nlen > a_bcap ? nlen + 1 : a_bcap);
        ast_print_buf = (char*)realloc(ast_print_buf, sizeof(char) * a_bcap);
    }
}

static void ast_print_buf_push(char *fmt, ...) {
    char buf[1025];
    va_list list;
    va_start(list, fmt);
    i32_t n = vsnprintf(buf, 1024, fmt, list);
    va_end(list);

    ast_print_buf_resize(a_blen + n + 1);
    memcpy(ast_print_buf+a_blen, buf, n);
    a_blen += n;
    ast_print_buf[a_blen] = '\0';
}

void ast_module_print(ast_node_t *r) {
    ast_module_t *m = (ast_module_t*)r;
    ast_print_buf_push("(module\n");
    ++indent;
    for (i32_t i = 0; i < m->size; ++i) {
        m->stmts[i]->print(m->stmts[i]);
    }
    --indent;
    ast_print_buf_push("module)\n");
}

void ast_module_delete(ast_node_t *r) {
    ast_module_t *m = (ast_module_t*)r;
    for (i32_t i = 0; i < m->size; ++i) {
        m->stmts[i]->delete(m->stmts[i]);
    }
    free(m->stmts);
    free(m);
}

void ast_module_evaluate(ast_node_t *r) {
    ast_module_t *m = (ast_module_t*)r;
    for (i32_t i = 0; i < m->size; ++i) {
        m->stmts[i]->evaluate(m->stmts[i]);
    }
}

ast_module_t *ast_module_new(void) {
    ast_module_t *_n = (ast_module_t*)malloc(sizeof(ast_module_t));
    *_n = (ast_module_t) {
        .base         = {
            .type     = ast_node_module,
            .print    = ast_module_print,
            .delete   = ast_module_delete,
            .evaluate = ast_module_evaluate,
        },
        .size         = 0,
        .cap          = 1,
        .stmts        = NULL,
    };
    ast_print_buf_new();
    return _n;
}

static void ast_module_resize(ast_module_t *module) {
#define AST_MAIN_GROW_FACTOR 2
    if (module->size + 1 >= module->cap) {
        module->cap *= AST_MAIN_GROW_FACTOR;
        module->stmts = (ast_node_t**)realloc(module->stmts,
                      sizeof(ast_node_t*) * module->cap);
    }
}

ast_node_t *ast_module_insert(ast_module_t *module, ast_node_t *stmt) {
    ast_module_resize(module);
    module->stmts[module->size++] = stmt;
    return (ast_node_t*)module;
}

void ast_stmt_print(ast_node_t *r) {
    ast_stmt_t *s = (ast_stmt_t*)r;
    ast_print_buf_push("%*s(stmt\n", indent*4, " ");
    ++indent;
    if (s->expr) s->expr->print(s->expr);
    --indent;
    ast_print_buf_push("%*sstmt)\n", indent*4, " ");
}

void ast_stmt_delete(ast_node_t *r) {
    ast_stmt_t *s = (ast_stmt_t*)r;
    if (s->expr) s->expr->delete(s->expr);
    free(s);
}

void ast_stmt_evaluate(ast_node_t *r) {
    ast_stmt_t *s = (ast_stmt_t*)r;
    if (s->expr) s->expr->evaluate(s->expr);
}

ast_stmt_t *ast_stmt_new(ast_node_t *expr) {
    ast_stmt_t *_n = (ast_stmt_t*)malloc(sizeof(ast_stmt_t));
    *_n = (ast_stmt_t) {
        .base         = {
            .type     = ast_node_stmt,
            .print    = ast_stmt_print,
            .delete   = ast_stmt_delete,
            .evaluate = ast_stmt_evaluate,
        },
        .expr         = expr,
    };
    return _n;
}

void ast_expr_print(ast_node_t *r) {
    ast_expr_t *e = (ast_expr_t*)r;
    ast_print_buf_push("%*s(expr\n", indent*4, " ");
    ++indent;
    if (e->term) e->term->print(e->term);
    --indent;
    ast_print_buf_push("%*sexpr)\n", indent*4, " ");
}

void ast_expr_delete(ast_node_t *r) {
    ast_expr_t *e = (ast_expr_t*)r;
    if (e->term) e->term->delete(e->term);
    free(e);
}

void ast_expr_evaluate(ast_node_t *r) {
    ast_expr_t *e = (ast_expr_t*)r;
    if (e->term) e->term->evaluate(e->term);
}

ast_expr_t *ast_expr_new(ast_node_t *term) {
    ast_expr_t *_n  = (ast_expr_t*)malloc(sizeof(ast_expr_t));
    *_n = (ast_expr_t) {
        .base         = {
            .type     = ast_node_expr,
            .print    = ast_expr_print,
            .delete   = ast_expr_delete,
            .evaluate = ast_expr_evaluate,
        },
        .term         = term,
    };
    return _n;
}

void ast_term_print(ast_node_t *r) {
    ast_term_t *t = (ast_term_t*)r;
    ast_print_buf_push("%*s(term\n", indent*4, " ");
    ++indent;
    if (t->left) t->left->print(t->left);
    if (t->right) t->right->print(t->right);
    --indent;
    ast_print_buf_push("%*s%s\n%*sterm)\n", (indent+1)*4, " ", ast_op_type_to_str[t->op], indent*4, " ");
}

void ast_term_delete(ast_node_t *r) {
    ast_term_t *t = (ast_term_t*)r;
    if (t->left) t->left->delete(t->left);
    if (t->right) t->right->delete(t->right);
    free(t);
}

void ast_term_evaluate(ast_node_t *r) {
    ast_term_t *t = (ast_term_t*)r;
    if (t->left) t->left->evaluate(t->left);
    if (t->right) t->right->evaluate(t->right);

    switch (t->op) {
    case ast_bin_op_plus:
        evaluate_add();
        break;
    case ast_bin_op_minus:
        evaluate_sub();
        break;
    default:
        break;
    }
}

ast_term_t *ast_term_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_term_t *_n = (ast_term_t*)malloc(sizeof(ast_term_t));
    *_n = (ast_term_t) {
        .base         = {
            .type     = ast_node_term,
            .print    = ast_term_print,
            .delete   = ast_term_delete,
            .evaluate = ast_term_evaluate,
        },
        .op           = op,
        .left         = left,
        .right        = right,
    };
    return _n;
}

void ast_factor_print(ast_node_t *r) {
    ast_factor_t *f = (ast_factor_t*)r;
    ast_print_buf_push("%*s(factor\n", indent*4, " ");
    ++indent;
    if (f->left) f->left->print(f->left);
    if (f->right) f->right->print(f->right);
    --indent;
    ast_print_buf_push("%*s%s\n%*sfactor)\n", 
                      (indent+1)*4, 
                      " ", 
                      ast_op_type_to_str[f->op], 
                      indent*4, 
                      " ");
}

void ast_factor_delete(ast_node_t *r) {
    ast_factor_t *f = (ast_factor_t*)r;
    if (f->left) f->left->delete(f->left);
    if (f->right) f->right->delete(f->right);
    free(f);
}

void ast_factor_evaluate(ast_node_t *r) {
    ast_factor_t *f = (ast_factor_t*)r;
    if (f->left) f->left->evaluate(f->left);
    if (f->right) f->right->evaluate(f->right);
    switch (f->op) {
    case ast_bin_op_star:
        evaluate_mul();
        break;
    case ast_bin_op_slash:
        evaluate_div();
        break;
    case ast_bin_op_mod:
        evaluate_mod();
        break;
    case ast_bin_op_pow:
        evaluate_pow();
        break;
    default:
        break;
    }
}

ast_factor_t *ast_factor_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_factor_t *_n = (ast_factor_t*)malloc(sizeof(ast_factor_t));
    *_n = (ast_factor_t) {
        .base         = {
            .type     = ast_node_factor,
            .print    = ast_factor_print,
            .delete   = ast_factor_delete,
            .evaluate = ast_factor_evaluate,
        },
        .op           = op,
        .left         = left,
        .right        = right,
    };
    return _n;
}

void ast_unary_print(ast_node_t *r) {
    ast_unary_t *u = (ast_unary_t*)r;
    ast_print_buf_push("%*s(unary\n", indent*4, " ");
    ++indent;
    if (u->expr) u->expr->print(u->expr);
    --indent;
    ast_print_buf_push("%*sunary)\n", indent*4, " ");
}

void ast_unary_delete(ast_node_t *r) {
    ast_unary_t *u = (ast_unary_t*)r;
    if (u->expr) u->expr->delete(u->expr);
    free(u);
}

void ast_unary_evaluate(ast_node_t *r) {
    ast_unary_t *u = (ast_unary_t*)r;
    if (u->expr) u->expr->evaluate(u->expr);
}

ast_unary_t *ast_unary_new(ast_node_t *expr) {
    ast_unary_t *_n = (ast_unary_t*)malloc(sizeof(ast_unary_t));
    *_n = (ast_unary_t) {
        .base         = {
            .type     = ast_node_unary,
            .print    = ast_unary_print,
            .delete   = ast_unary_delete,
            .evaluate = ast_unary_evaluate,
        },
        .op           = -1,
        .expr         = expr,
    };
    return _n;
}

void ast_atom_print(ast_node_t *r) {
    ast_atom_t *a = (ast_atom_t*)r;
    ast_print_buf_push("%*s(atom\n", indent*4, " ");
    ++indent;
    if (a->atom) a->atom->print(a->atom);
    --indent;
    ast_print_buf_push("%*satom)\n", indent*4, " ");
}

void ast_atom_delete(ast_node_t *r) {
    ast_atom_t *a = (ast_atom_t*)r;
    if (a->atom) a->atom->delete(a->atom);
    free(a);
}

void ast_atom_evaluate(ast_node_t *r) {
    ast_atom_t *a = (ast_atom_t*)r;
    if (a->atom) a->atom->evaluate(a->atom);
}

ast_atom_t *ast_atom_new(ast_node_t *atom) {
    ast_atom_t *_n = (ast_atom_t*)malloc(sizeof(ast_atom_t));
    *_n = (ast_atom_t) {
        .base         = {
            .type     = ast_node_atom,
            .print    = ast_atom_print,
            .delete   = ast_atom_delete,
            .evaluate = ast_atom_evaluate,
        },
        .atom         = atom,
    };
    return _n;
}

void ast_number_print(ast_node_t *r) {
    ast_number_t *n = (ast_number_t*)r;
    if (n->base.type == ast_node_inumber)
        ast_print_buf_push("%*s%lld\n", indent*4, " ", (long long)n->num.i);
    else
     ast_print_buf_push("%*s%lf\n", indent*4, " ", (double)n->num.f);
}

void ast_number_delete(ast_node_t *r) {
    free(r);
}

void ast_number_evaluate(ast_node_t *r) {
    ast_number_t *n = (ast_number_t*)r;
    if (n->base.type == ast_node_inumber) {
        evaluate_int(n->num.i); 
    } else {
        evaluate_float(n->num.f);
    }
}

ast_number_t *ast_number_new(ast_number_internal_t num, ast_node_type_t type) {
    ast_number_t *_n = (ast_number_t*)malloc(sizeof(ast_number_t));
    *_n = (ast_number_t) {
        .base         = {
            .type     = type,
            .print    = ast_number_print,
            .delete   = ast_number_delete,
            .evaluate = ast_number_evaluate,
        },
        .num          = num,
    };
    return _n;
}

void ast_list_print(ast_node_t *r) {
    ast_list_t *m = (ast_list_t*)r;
    ast_print_buf_push("%*s(list\n", indent*4, " ");
    ++indent;
    for (i32_t i = 0; i < m->len; ++i) {
        m->elems[i]->print(m->elems[i]);
    }
    --indent;
    ast_print_buf_push("%*slist)\n", indent*4, " ");
}

void ast_list_delete(ast_node_t *r) {
    ast_list_t *m = (ast_list_t*)r;
    for (i32_t i = 0; i < m->len; ++i) {
        m->elems[i]->delete(m->elems[i]);
    }
    free(m->elems);
    free(m);
}

void ast_list_evaluate(ast_node_t *r) {
    ast_list_t *m = (ast_list_t*)r;
    evaluate_list(m);
}

static void ast_list_resize(ast_list_t *list) {
#define AST_MAIN_GROW_FACTOR 2
    if (list->len + 1 >= list->cap) {
        list->cap  *= AST_MAIN_GROW_FACTOR;
        list->elems = (ast_node_t**)realloc(list->elems,
                      sizeof(ast_node_t*) * list->cap);
    }
}

ast_node_t *ast_list_insert(ast_list_t *list, ast_node_t *elem) {
    ast_list_resize(list);
    list->elems[list->len++] = elem;
    return (ast_node_t*)list;
}

ast_list_t *ast_list_new(void) {
    ast_list_t *_n = (ast_list_t*)malloc(sizeof(ast_list_t));
    *_n = (ast_list_t) {
        .base         = {
            .type     = ast_node_list,
            .print    = ast_list_print,
            .delete   = ast_list_delete,
            .evaluate = ast_list_evaluate,
        },
        .len          = 0,
        .cap          = 1,
        .elems        = NULL,
    };
    ast_print_buf_new();
    return _n;
}


void ast_print_buf_new(void) {
    ast_print_buf_delete(); 
}

void ast_print_buf_delete(void) {
    if (ast_print_buf) free(ast_print_buf);
    ast_print_buf = NULL;
    a_blen = 0;
    a_bcap = 1;
}

ast_op_type_t ast_convert_toktype_to_astoptype(tokentype_t op) {
    if (op == t_plus)  return ast_bin_op_plus;
    if (op == t_minus) return ast_bin_op_minus;
    if (op == t_star)  return ast_bin_op_star;
    if (op == t_slash) return ast_bin_op_slash;
    if (op == t_mod)   return ast_bin_op_mod;
    if (op == t_pow)   return ast_bin_op_pow;
    return ast_op_unknown;
}
