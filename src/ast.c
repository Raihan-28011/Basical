/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#include "ast.h"
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

i16_t indent = 0;
char *ast_print_buf = NULL;
i16_t a_blen = 0;
i16_t a_bcap = 1;

static void ast_print_buf_resize(i16_t nlen) {
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
    i16_t n = vsnprintf(buf, 1024, fmt, list);
    va_end(list);

    ast_print_buf_resize(a_blen + n);
    memcpy(ast_print_buf+a_blen, buf, n);
    a_blen += n;
    ast_print_buf[a_blen] = '\0';
}

void ast_package_print(ast_node_t *r) {
    ast_package_t *m = (ast_package_t*)r;
    ast_print_buf_push("(package\n");
    ++indent;
    for (i16_t i = 0; i < m->size; ++i) {
        m->stmts[i]->print(m->stmts[i]);
    }
    --indent;
    ast_print_buf_push("package)\n");
}

void ast_package_delete(ast_node_t *r) {
    ast_package_t *m = (ast_package_t*)r;
    for (i16_t i = 0; i < m->size; ++i) {
        m->stmts[i]->delete(m->stmts[i]);
    }
    free(m->stmts);
    free(m);
}

ast_package_t *ast_package_new(void) {
    ast_package_t *_n = (ast_package_t*)malloc(sizeof(ast_package_t));
    *_n = (ast_package_t) {
        .base        = {
            .type    = ast_node_package,
            .print   = ast_package_print,
            .delete  = ast_package_delete,
        },
        .size       = 0,
        .cap        = 1,
        .stmts      = NULL,
    };
    ast_print_buf_new();
    return _n;
}

static void ast_package_resize(ast_package_t *package) {
#define AST_MAIN_GROW_FACTOR 2
    if (package->size + 1 >= package->cap) {
        package->cap *= AST_MAIN_GROW_FACTOR;
        package->stmts = (ast_node_t**)realloc(package->stmts,
                      sizeof(ast_node_t*) * package->cap);
    }
}

ast_node_t *ast_package_insert(ast_package_t *package, ast_node_t *stmt) {
    ast_package_resize(package);
    package->stmts[package->size++] = stmt;
    return (ast_node_t*)package;
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

ast_stmt_t *ast_stmt_new(ast_node_t *expr) {
    ast_stmt_t *_n = (ast_stmt_t*)malloc(sizeof(ast_stmt_t));
    *_n = (ast_stmt_t) {
        .base       = {
            .type   = ast_node_stmt,
            .print  = ast_stmt_print,
            .delete = ast_stmt_delete,
        },
        .expr       = expr,
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

ast_expr_t *ast_expr_new(ast_node_t *term) {
    ast_expr_t *_n  = (ast_expr_t*)malloc(sizeof(ast_expr_t));
    *_n = (ast_expr_t) {
        .base       = {
            .type   = ast_node_expr,
            .print  = ast_expr_print,
            .delete = ast_expr_delete,
        },
        .term       = term,
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

ast_term_t *ast_term_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_term_t *_n = (ast_term_t*)malloc(sizeof(ast_term_t));
    *_n = (ast_term_t) {
        .base       = {
            .type   = ast_node_term,
            .print  = ast_term_print,
            .delete = ast_term_delete,
        },
        .op         = op,
        .left       = left,
        .right      = right,
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

ast_factor_t *ast_factor_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_factor_t *_n = (ast_factor_t*)malloc(sizeof(ast_factor_t));
    *_n = (ast_factor_t) {
        .base        = {
            .type    = ast_node_factor,
            .print   = ast_factor_print,
            .delete  = ast_factor_delete,
        },
        .op          = op,
        .left        = left,
        .right       = right,
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

ast_unary_t *ast_unary_new(ast_node_t *expr) {
    ast_unary_t *_n = (ast_unary_t*)malloc(sizeof(ast_unary_t));
    *_n = (ast_unary_t) {
        .base        = {
            .type    = ast_node_unary,
            .print   = ast_unary_print,
            .delete  = ast_unary_delete,
        },
        .op          = -1,
        .expr        = expr,
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

ast_number_t *ast_number_new(ast_number_internal_t num, ast_node_type_t type) {
    ast_number_t *_n = (ast_number_t*)malloc(sizeof(ast_number_t));
    *_n = (ast_number_t) {
        .base       = {
            .type   = type,
            .print  = ast_number_print,
            .delete = ast_number_delete,
        },
        .num        = num,
    };
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
    if (op == t_plus) return ast_bin_op_plus;
    if (op == t_minus) return ast_bin_op_minus;
    if (op == t_star) return ast_bin_op_star;
    if (op == t_slash) return ast_bin_op_slash;
    if (op == t_mod) return ast_bin_op_mod;
    if (op == t_pow) return ast_bin_op_pow;
    return ast_op_unknown;
}
