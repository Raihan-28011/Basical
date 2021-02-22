//
// Created by raihan on 2/22/21.
//

#include "include/parser.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "expression.h"
#include "include/error.h"

Token cur_token(Parser *parser) {
  return next_token(&parser->scanner);
}

Token peek_token(Parser *parser) {
  char *start = parser->scanner._start;
  char *current = parser->scanner._current;

  Token ret = next_token(&parser->scanner);
  parser->scanner._start = start;
  parser->scanner._current = current;

  return ret;
}

bool check_token(Parser *parser, int tokNum, ...) {
  va_list list;
  va_start(list, tokNum);
  bool ret = false;
  
  while (tokNum--) {
    if (ret)
      return ret;
  
    TokenType tok = va_arg(list, TokenType);
    ret = (ret || (peek_token(parser)._type == tok));
  }
  
  va_end(list);
  return ret;
}

static Expression *parse_term(Parser *parser);

static Expression *parse_primary(Parser *parser) {
  if (check_token(parser, 2, TOKEN_INT_LITERAL, TOKEN_FLOAT_LITERAL)) {
    return make_literal_expression(cur_token(parser));
  }

  if (check_token(parser, 1, TOKEN_LEFT_PAREN)) {
    cur_token(parser);
    Expression *left = parse_term(parser);
  
    if (check_token(parser, 1, TOKEN_RIGHT_PAREN))
      cur_token(parser);
    else {
      add_error_token("expected ')', found '%*s'", peek_token(parser)._len, cur_token(parser)._start);
      return NULL_EXPR_PTR;
    }
    return make_grouped_expression(left);
  }
  
  Token tok;
  if ((tok = cur_token(parser))._type == TOKEN_UNRECOGNISED) {
      add_error_token("unrecognised token '%*s'", tok._len, tok._start);
      /* free(tok._start); */
  } else if (tok._type == TOKEN_EOF) {
      add_error_token("unexpected 'eof' found");
  } else {
    add_error_token("expected expression, found '%*s'", tok._len, tok._start);
  }
  return NULL_EXPR_PTR;
}

static Expression *parse_unary(Parser *parser) {
  if (check_token(parser, 1, TOKEN_MINUS)) {
    Token optok = cur_token(parser);
    Expression *left = parse_term(parser);
    return make_unary_expression(optok, left);
  }
  return parse_primary(parser);
}

static Expression *parse_factor(Parser *parser) {
  Expression *left = parse_unary(parser);

  while (check_token(parser, 3, TOKEN_STAR, TOKEN_SLASH, TOKEN_POW)) {
    Token optok = cur_token(parser);
    Expression *right = parse_unary(parser);
    left = make_binary_expression(left, optok, right);
  }
  return left;
}

Expression *parse_term(Parser *parser) {
  Expression *left = parse_factor(parser);

  while (check_token(parser, 2, TOKEN_PLUS, TOKEN_MINUS)) {
    Token optok = cur_token(parser);
    Expression *right = parse_factor(parser);
    left = make_binary_expression(left, optok, right);
  }
  return left;
}

static void parse_expression(Parser *parser, ExprTree *tree) {
  tree->_root = parse_term(parser);
}

ExprTree *parse(Parser *parser, char const *source) {
  scanner_init(&parser->scanner, (char*)source);

  ExprTree *tree = (ExprTree*)malloc(sizeof(ExprTree));
  parse_expression(parser, tree);

  return tree;
}
