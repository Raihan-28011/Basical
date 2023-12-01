/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "token.h"

char const *token_to_str[] = {
  "t_plus", "t_minus", "t_star", "t_slash",
  "t_mod", "t_pow", 
  "t_lparen", "t_rparen",
  "t_iliteral", "t_fliteral", 
  "t_newline", "t_eof", "t_unrecognised"
};

void token_print(token_t *token) {
    printf("%s ", token_to_str[token->type]);
    if (token->type == t_newline || token->type == t_eof) printf("\n");
}
