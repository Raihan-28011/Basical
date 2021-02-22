//
// Created by raihan on 2/22/21.
//

#include "include/scanner.h"
#include <string.h>
#include <stdbool.h>

static bool is_end(Scanner *scanner) {
  return scanner->_current - scanner->_source >= scanner->_len;
}

static char advance(Scanner *scanner) {
  if (is_end(scanner))
    return '\0';
  return *scanner->_current++;
}

static void skip_whitespace(Scanner *scanner) {
  while (isspace(*scanner->_current))
    advance(scanner);
}

char peek(Scanner *scanner, int offset) {
  return *(scanner->_current + offset);
}

static Token eof_token() {
  return (Token) { TOKEN_EOF, "", 0 };
}

static Token make_token(TokenType type, char *start, size_t len) {
  return (Token) { type, start, len};
}

static Token number_token(Scanner *scanner) {
  while (isdigit(peek(scanner, 0)) && !is_end(scanner))
    advance(scanner);
  if (peek(scanner, 0) == '.') {
    floatPresent = true;
    advance(scanner);

    while (isdigit(peek(scanner, 0)))
      advance(scanner);
  }

#define len() (scanner->_current - scanner->_start)
  return ((floatPresent) ?
          make_token(TOKEN_FLOAT_LITERAL, scanner->_current, len())
          : make_token(TOKEN_INT_LITERAL, scanner->_current, len()));
}

Token next_token(Scanner *scanner) {
  if (is_end(scanner))
    return eof_token();

  scanner->_start = scanner->_current;
  skip_whitespace(scanner);

  char c = advance(scanner);
  switch (c) {
    case '+': return make_token(TOKEN_PLUS, "+", 1);
    case '-': return make_token(TOKEN_MINUS, "-", 1);
    case '*':
      if (peek(scanner, 0) == *)
        return make_token(TOKEN_POW, "**", 2);
      return make_token(TOKEN_STAR, "*", 1);
    case '/': return make_token(TOKEN_SLASH, "/", 1);
    case '%': return make_token(TOKEN_MOD, "%", 1);
    case '(': return make_token(TOKEN_LEFT_PAREN, "(", 1);
    case ')': return make_token(TOKEN_RIGHT_PAREN, ")", 1);
    default:
      if (isdigit(c))
        return number_token(scanner);
      make_token(TOKEN_UNRECOGNISED, "", 0);
  }
}
