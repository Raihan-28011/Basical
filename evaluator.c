//
// Created by raihan on 2/22/21.
//

#include "include/evaluator.h"
#include "include/scanner.h"
#include <stdlib.h>
#include "include/error.h"
#include <math.h>

double evaluate_expression_d(Expression *root) {
  if (root == NULL_EXPR_PTR)
    return -1.0;
  if (error_occurred)
    return -1.0;

  double val = 0.0;
  double val2 = 0.0;
  switch (root->_type) {
    case EXPR_LITERAL:
       val = strtod(root->_optok._start, NULL);
       break;
    case EXPR_UNARY:
      val = evaluate_expression_d(root->_right);
      switch (root->_optok._type) {
        case TOKEN_MINUS:
          val = -val;
          break;
        default:
          break;
      }
      break;
    case EXPR_BINARY:
      val = evaluate_expression_d(root->_left);
      val2 = evaluate_expression_d(root->_right);
      switch (root->_optok._type) {
        case TOKEN_PLUS:
          val = val + val2;
          break;
        case TOKEN_MINUS:
          val = val - val2;
          break;
        case TOKEN_STAR:
          val = val * val2;
          break;
        case TOKEN_SLASH:
          if (val2 == 0.0) {
            add_error("dividing expression by '0' is invalid operation");
            return -1.0;
          }
          val = val / val2;
          break;
        case TOKEN_MOD:
          val = fmod(val, val2);
          break;
        case TOKEN_POW:
          val = pow(val, val2);
          break;
        default:
          break;
      }
      break;
    case EXPR_GROUPED:
      val = evaluate_expression_d(root->_left);
      break;
    default:
      val = -1.0;
      break;
  }
  return val;
}


long long evaluate_expression_l(Expression *root) {
  if (root == NULL_EXPR_PTR)
    return -1;
  if (error_occurred)
    return -1;

  long long val = 0;
  long long val2 = 0;
  switch (root->_type) {
    case EXPR_LITERAL:
      val = strtoll(root->_optok._start, NULL, 10);
      break;
    case EXPR_UNARY:
      val = evaluate_expression_l(root->_right);
      switch (root->_optok._type) {
        case TOKEN_MINUS:
          val = -val;
          break;
        default:
          break;
      }
      break;
    case EXPR_BINARY:
      val = evaluate_expression_l(root->_left);
      val2 = evaluate_expression_l(root->_right);
      switch (root->_optok._type) {
        case TOKEN_PLUS:
          val = val + val2;
          break;
        case TOKEN_MINUS:
          val = val - val2;
          break;
        case TOKEN_STAR:
          val = val * val2;
          break;
        case TOKEN_SLASH:
          if (val2 == 0) {
            add_error("dividing expression by '0' is invalid operation");
            return -1;
          }
          val = val / val2;
          break;
        case TOKEN_MOD:
          val = val % val2;
          break;
        case TOKEN_POW:
        {
          long long pow = val2;
          long long i = val;
          while (--pow)
            val *= i;
        }
        break;
        default:
          break;
      }
      break;
    case EXPR_GROUPED:
      val = evaluate_expression_l(root->_left);
      break;
    default:
      val = -1;
      break;
  }
  return val;
}
