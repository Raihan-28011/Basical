/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#ifndef __BASICAL_AST_H__
#define __BASICAL_AST_H__

#include "common.h"
#include "token.h"

typedef struct ast_node ast_node_t;
typedef struct ast_module ast_module_t;
typedef struct ast_stmt ast_stmt_t;
typedef struct ast_expr ast_expr_t;
typedef struct ast_term ast_term_t;
typedef struct ast_factor ast_factor_t;
typedef struct ast_unary ast_unary_t;
typedef struct ast_number ast_number_t;
typedef union  ast_number_internal ast_number_internal_t;
typedef enum ast_node_type {
    ast_node_module,
    ast_node_stmt,
    ast_node_expr,
    ast_node_term,
    ast_node_factor,
    ast_node_unary, 
    ast_node_inumber,
    ast_node_fnumber,
} ast_node_type_t;

typedef enum ast_op_type {
  ast_bin_op_plus, 
  ast_bin_op_minus, 
  ast_bin_op_star, 
  ast_bin_op_slash,
  ast_bin_op_mod, 
  ast_bin_op_pow,
  ast_op_unknown,
} ast_op_type_t;

struct ast_node {
    ast_node_type_t type;
    void (*print)(ast_node_t *r);
    void (*delete)(ast_node_t *r);
};

struct ast_module {
    ast_node_t base;
    i16_t      size;
    i16_t      cap;
    ast_node_t **stmts;
};

struct ast_stmt {
    ast_node_t base;
    ast_node_t *expr;
};

struct ast_expr {
    ast_node_t base;
    ast_node_t *term;
};

struct ast_term {
    ast_node_t    base;
    ast_op_type_t op;
    ast_node_t    *left;
    ast_node_t    *right;
};

struct ast_factor {
    ast_node_t    base;
    ast_op_type_t op;
    ast_node_t    *left;
    ast_node_t    *right;
};

struct ast_unary {
    ast_node_t    base;
    ast_op_type_t op;
    ast_node_t    *expr;
};

union ast_number_internal {
    i64_t i;
    f64_t f;
};

struct ast_number {
    ast_node_t            base;
    ast_number_internal_t num;
};

extern char *ast_print_buf;
extern i16_t a_blen;

ast_module_t *ast_module_new(void);
ast_node_t   *ast_module_insert(ast_module_t *main, ast_node_t *stmt);
void         ast_module_print(ast_node_t *r);
void         ast_module_delete(ast_node_t *r);

ast_stmt_t *ast_stmt_new(ast_node_t *expr);
void       ast_stmt_print(ast_node_t *r);
void       ast_stmt_delete(ast_node_t *r);

ast_expr_t *ast_expr_new(ast_node_t *term);
void       ast_expr_print(ast_node_t *r);
void       ast_expr_delete(ast_node_t *r);

ast_term_t *ast_term_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op);
void       ast_term_print(ast_node_t *r);
void       ast_term_delete(ast_node_t *r);

ast_factor_t *ast_factor_new(ast_node_t *left, ast_node_t *right, ast_op_type_t op);
void         ast_factor_print(ast_node_t *r);
void         ast_factor_delete(ast_node_t *r);

ast_unary_t *ast_unary_new(ast_node_t *expr);
void        ast_unary_print(ast_node_t *r);
void        ast_unary_delete(ast_node_t *r);

ast_number_t *ast_number_new(ast_number_internal_t num, ast_node_type_t type);
void         ast_number_print(ast_node_t *r);
void         ast_number_delete(ast_node_t *r);

void          ast_print_buf_new(void);
void          ast_print_buf_delete(void);
ast_op_type_t ast_convert_toktype_to_astoptype(tokentype_t op);
#endif // __BASICAL_AST_H__
