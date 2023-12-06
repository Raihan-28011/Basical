/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#include "ast.h"
#include "token.h"
#include <stdlib.h>

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

ast_node_t ast_node_new(ast_node_type_t type) {
    ast_node_t _n;
    _n.type       = type;
    _n.print      = NULL;
    _n.delete     = NULL;
    return _n;
}

void ast_main_print(ast_node_t *r) {
    ast_main_t *m = (ast_main_t*)r;
    fprintf(stderr, "{\n");
    ++indent;
    for (i16_t i = 0; i < m->size; ++i) {
        m->stmts[i]->print(m->stmts[i]);
    }
    --indent;
    fprintf(stderr, "}\n");
}

void ast_main_delete(ast_node_t *r) {
    ast_main_t *m = (ast_main_t*)r;
    for (i16_t i = 0; i < m->size; ++i) {
        m->stmts[i]->delete(m->stmts[i]);
    }
    free(m->stmts);
    free(m);
}

ast_main_t *ast_main_new(void) {
    ast_main_t *_n = (ast_main_t*)malloc(sizeof(ast_main_t));
    _n->base       = ast_node_new(AST_MAIN);
    _n->base.print = ast_main_print;
    _n->base.delete = ast_main_delete;
    _n->size       = 0;
    _n->cap        = 1;
    _n->stmts      = NULL;
    return _n;
}

static void ast_main_resize(ast_main_t *main) {
#define AST_MAIN_GROW_FACTOR 2
    if (main->size + 1 >= main->cap) {
        main->cap *= AST_MAIN_GROW_FACTOR;
        main->stmts = (ast_node_t**)realloc(main->stmts,
                      sizeof(ast_node_t*) * main->cap);
    }
}

ast_node_t *ast_main_insert(ast_main_t *main, ast_node_t *stmt) {
    ast_main_resize(main);
    main->stmts[main->size++] = stmt;
    return (ast_node_t*)main;
}

void ast_stmt_print(ast_node_t *r) {
    ast_stmt_t *s = (ast_stmt_t*)r;
    fprintf(stderr, "%*s(stmt ", indent*4, " ");
    s->expr->print(s->expr);
    fprintf(stderr, ")\n");
}

void ast_stmt_delete(ast_node_t *r) {
    ast_stmt_t *s = (ast_stmt_t*)r;
    s->expr->delete(s->expr);
    free(s);
}

ast_stmt_t *ast_stmt_new(ast_node_t *expr) {
    ast_stmt_t *_n = (ast_stmt_t*)malloc(sizeof(ast_stmt_t));
    _n->base       = ast_node_new(AST_STMT);
    _n->base.print = ast_stmt_print;
    _n->base.delete = ast_stmt_delete;
    _n->expr       = expr;
    return _n;
}

void ast_expr_print(ast_node_t *r) {
    ast_expr_t *e = (ast_expr_t*)r;
    fprintf(stderr, "(expr ");
    e->term->print(e->term);
    fprintf(stderr, ")");
}

void ast_expr_delete(ast_node_t *r) {
    ast_expr_t *e = (ast_expr_t*)r;
    e->term->delete(e->term);
    free(e);
}

ast_expr_t *ast_expr_new(ast_node_t *term) {
    ast_expr_t *_n = (ast_expr_t*)malloc(sizeof(ast_expr_t));
    _n->base       = ast_node_new(AST_EXPR);
    _n->base.print = ast_expr_print;
    _n->base.delete = ast_expr_print;
    _n->term       = term;
    return _n;
}

void ast_term_print(ast_node_t *r) {
    ast_term_t *t = (ast_term_t*)r;
    fprintf(stderr, "(term ");
    t->left->print(t->left);
    t->right->print(t->right);
    fprintf(stderr, "%s)", ast_op_type_to_str[t->op]);
}

void ast_term_delete(ast_node_t *r) {
    ast_term_t *t = (ast_term_t*)r;
    t->left->delete(t->left);
    t->right->delete(t->right);
    free(t);
}

ast_term_t *ast_term_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_term_t *_n = (ast_term_t*)malloc(sizeof(ast_term_t));
    _n->base       = ast_node_new(AST_TERM);
    _n->base.print = ast_term_print;
    _n->base.delete = ast_term_delete;
    _n->op         = op;
    _n->left       = left;
    _n->right      = right;
    return _n;
}

void ast_factor_print(ast_node_t *r) {
    ast_factor_t *f = (ast_factor_t*)r;
    fprintf(stderr, "(factor ");
    f->left->print(f->left);
    f->right->print(f->right);
    fprintf(stderr, "%s)", ast_op_type_to_str[f->op]);
}

void ast_factor_delete(ast_node_t *r) {
    ast_factor_t *f = (ast_factor_t*)r;
    f->left->delete(f->left);
    f->right->delete(f->right);
    free(f);
}

ast_factor_t *ast_factor_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op) {
    ast_factor_t *_n = (ast_factor_t*)malloc(sizeof(ast_factor_t));
    _n->base        = ast_node_new(AST_FACTOR); 
    _n->base.print = ast_factor_print;
    _n->base.delete = ast_factor_delete;
    _n->op          = op;
    _n->left        = left;
    _n->right       = right;
    return _n;
}

void ast_unary_print(ast_node_t *r) {
    ast_unary_t *u = (ast_unary_t*)r;
    fprintf(stderr, "(unary ");
    u->expr->print(u->expr);
    fprintf(stderr, ")");
}

void ast_unary_delete(ast_node_t *r) {
    ast_unary_t *u = (ast_unary_t*)r;
    u->expr->delete(u->expr);
    free(u);
}

ast_unary_t *ast_unary_new(ast_node_t *expr) {
    ast_unary_t *_n = (ast_unary_t*)malloc(sizeof(ast_unary_t));
    _n->base        = ast_node_new(AST_UNARY);
    _n->base.print = ast_unary_print;
    _n->base.delete = ast_unary_delete;
    _n->op          = -1;
    _n->expr        = expr;
    return _n;
}

void ast_number_print(ast_node_t *r) {
    ast_number_t *n = (ast_number_t*)r;
    if (n->base.type == AST_INUMBER)
        fprintf(stderr, "%lld ", (long long)n->num.i);
    else
     fprintf(stderr, "%lf ", (double)n->num.f);
}

void ast_number_delete(ast_node_t *r) {
    fprintf(stderr, "called number delete\n");
    free(r);
}

ast_number_t *ast_number_new(ast_number_internal_t num, ast_node_type_t type) {
    ast_number_t *_n = (ast_number_t*)malloc(sizeof(ast_number_t));
    _n->base       = ast_node_new(type);
    _n->base.print = ast_number_print;
    _n->base.delete = ast_number_delete;
    _n->num        = num;
    return _n;
}

ast_op_type_t ast_convert_toktype_to_astoptype(tokentype_t op) {
    if (op == t_plus) return AST_BIN_OP_PLUS;
    if (op == t_minus) return AST_BIN_OP_MINUS;
    if (op == t_star) return AST_BIN_OP_STAR;
    if (op == t_slash) return AST_BIN_OP_SLASH;
    if (op == t_mod) return AST_BIN_OP_MOD;
    if (op == t_pow) return AST_BIN_OP_POW;
    return AST_OP_UNKNOWN;
}
