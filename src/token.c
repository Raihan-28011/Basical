/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "token.h"
#include <stdio.h>

char const *token_to_str[] = {
  "t_eof", "t_newline", "t_unrecognised",
  "t_plus", "t_minus", "t_star", "t_slash",
  "t_mod", "t_pow", 
  "t_lparen", "t_rparen",
  "t_iliteral", "t_fliteral", 
};

void token_print(token_t *token) {
    fprintf(stderr, "%s(%d:%d:%d) ", 
            token_to_str[token->type], 
            token->ln, 
            token->col, 
            token->len);
    if (token->type == t_newline || token->type == t_eof) fprintf(stderr, "\n");
}
