//
// Created by raihan on 2/22/21.
//

#include "include/expression.h"

#include <stdlib.h>

Expression *expression_new() {
  Expression *new = (Expression*)malloc(sizeof(Expression));
  new->_type = EXPR_NULL;
  new->_optok = (Token) { TOKEN_EOF, "", 0 } ;
  new->_left = new->_right = NULL_EXPR_PTR;

  return new;
}

Expression *make_literal_expression(Token optok) {
  Expression *expr = expression_new();
  expr->_type = EXPR_LITERAL;
  expr->_optok = optok;

  return expr;
}

Expression *make_unary_expression(Token optok, Expression *right) {
  Expression *expr = expression_new();
  expr->_type = EXPR_UNARY;
  expr->_optok = optok;
  expr->_right = right;

  return expr;
}

Expression *make_binary_expression(Expression *left, Token optok, Expression *right) {
  Expression *expr = expression_new();
  expr->_type = EXPR_BINARY;
  expr->_optok = optok;
  expr->_left = left;
  expr->_right = right;

  return expr;
}

Expression *make_grouped_expression(Expression *left) {
  Expression *expr = expression_new();
  expr->_type = EXPR_GROUPED;
  expr->_left = left;

  return expr;
}

static void delete_expression(Expression *root) {
  if (root == NULL_EXPR_PTR)
    return;
  switch (root->_type) {
    case EXPR_LITERAL:
      free(root);
      return;
    case EXPR_UNARY:
      delete_expression(root->_right);
      free(root);
      return;
    case EXPR_BINARY:
      delete_expression(root->_left);
      delete_expression(root->_right);
      free(root);
      return;
    case EXPR_GROUPED:
      delete_expression(root->_left);
      free(root);
      return;
    default:
      return;
  }
}

void delete_expression_tree(ExprTree *tree) {
  delete_expression(tree->_root);
  if (tree)
    free(tree);
}
