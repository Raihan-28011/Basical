/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "token.h"

void token_print(token_t *token) {
    switch (token->type) {
    case t_plus:
        printf("%s ", "t_plus");
        break;
    case t_minus:
        printf("%s ", "t_minus");
        break;
    case t_star:
        printf("%s ", "t_star");
        break;
    case t_slash:
        printf("%s ", "t_slash");
        break;
    case t_mod:
        printf("%s ", "t_mod");
        break;
    case t_pow:
        printf("%s ", "t_pow");
        break;
    case t_lparen:
    case t_rparen:
    case t_iliteral:
    case t_fliteral:
        printf("%.*s ", token->len, token->token);
        break;
    case t_newline:
        printf("\n");
        break;
    case t_eof:
        printf("t_eof\n");
        break;
    case t_unrecognised:
        printf("%.*s ", token->len, token->token);
        break;
    default:
        break;
    }
}
