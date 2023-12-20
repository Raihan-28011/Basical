/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#ifndef __BASICAL_LEXER_H__
#define __BASICAL_LEXER_H__

#include "common.h"
#include "token.h"
#include <stdbool.h>

#define LEXER_GROW_FACTOR 2

typedef struct lexer lexer_t;
struct lexer {
    i32_t      tcount;          // No. of tokens
    i32_t      cap;             // Token Capacity
    i32_t      line;            // Line no.
    i32_t      col;             // Column no.
    i32_t      pos;             // Current position in src
    i32_t      slen;            // Source string length
    bool       error_occured;   // Error occured during lexing
    char const *src;            // Source string
    char const *fname;          // File name
    token_t    *tokens;         // Array of tokens
};

lexer_t *lexer_new(void);
void    lexer_tokenize_string(lexer_t *lexer, char const *src, char const *fname);
void    lexer_tokenize_file(lexer_t *lexer, char const *fname);
token_t lexer_get_token(lexer_t const *lexer, i32_t index);
void    lexer_print_tokens(lexer_t *lexer);
void    lexer_delete(lexer_t *lexer);

#endif // __BASICAL_LEXER_H__
