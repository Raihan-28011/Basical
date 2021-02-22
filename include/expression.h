//
// Created by raihan on 2/22/21.
//

#ifndef BASICAL_EXPRESSION_H
#define BASICAL_EXPRESSION_H

#include "token.h"

typedef enum ExprType {
  EXPR_LITERAL,
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_GROUPED,
  EXPR_NULL
} ExprType;

typedef struct Expression {
  ExprType _type;   /* type of expression */
  Token _optok;   /* operator */
  struct Expression *_left;  /* left operand */
  struct Expression *_right;  /* right operand */
} Expression;

#define NULL_EXPR_PTR (Expression*)0

typedef struct ExprTree {
  Expression *_root;
} ExprTree;

Expression *expression_new();
Expression *make_literal_expression(Token optok);
Expression *make_unary_expression(Token optok, Expression *right);
Expression *make_binary_expression(Expression *left, Token optok, Expression *right);
Expression *make_grouped_expression(Expression *left);
void delete_expression_tree(ExprTree *tree);

#endif //BASICAL_EXPRESSION_H
