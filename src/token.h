/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#ifndef __BASICAL_TOKEN_H__
#define __BASICAL_TOKEN_H__

#include "common.h"

typedef enum tokentype {
  /* Special tokens */
  t_eof, t_newline, t_unrecognized,

  /* Operators */
  t_plus, t_minus, t_star, t_slash,
  t_mod, t_pow, 

  /* Delimeters */
  t_lparen, t_rparen,
  t_lsqbrace, t_rsqbrace,
  t_comma,

 /* Literals */
  t_iliteral, t_fliteral,
} tokentype_t;

typedef struct token token_t;
struct token {
    i32_t       len;     // Length of the token
    i32_t       ln, col; // Line and column of the token
    tokentype_t type;    // Token type
    char        *token;  // Token string
};

void token_print(token_t *token);

#endif // __BASICAL_TOKEN_H__
