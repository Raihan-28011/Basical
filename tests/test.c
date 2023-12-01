#include "test.h"
#include "../src/lexer.h"
#include <stdio.h>

#define ARRAY_SIZE(arr) (i32_t)(sizeof(arr)/sizeof(*arr))

tester_t g_tester = {
    .tests  = 0,
    .errors = 0,
};

static i32_t test_lexer_tokenize_string(void) {
    lexer_t *lexer = lexer_new();
    lexer_tokenize_string(lexer, "(132322 +4454545        ) \n\n\n* 3433");

    tokentype_t expected[] = {
        t_lparen, t_iliteral, t_plus, t_iliteral, t_rparen, 
        t_newline, 
        t_newline, 
        t_newline,
        t_star, t_iliteral, t_eof,
    };

    if (ARRAY_SIZE(expected) != lexer->size) return -1;
    for (i32_t i = 0; i < ARRAY_SIZE(expected); ++i) {
        if (lexer->tokens[i].type != expected[i]) return -1;
    }

    lexer_delete(lexer);
    return 0;
}

static i32_t test_lexer_tokenize_file(void) {
    lexer_t *lexer = lexer_new();
    lexer_tokenize_file(lexer, "../tests/tokenize.math");

    tokentype_t expected[] = {
        t_lparen, t_iliteral,  t_plus, t_iliteral,  t_rparen, 
        t_star, t_fliteral,  t_slash, t_iliteral,  t_mod, 
        t_lparen, t_iliteral,  t_pow, t_iliteral,  t_rparen,
        t_newline,
        t_iliteral,  t_plus, t_fliteral,  t_pow, t_lparen, 
        t_iliteral,  t_star, t_fliteral,  t_rparen, 
        t_mod, t_iliteral,  t_slash, t_iliteral, 
        t_newline,
        t_unrecognised, 
        t_lparen, t_fliteral,  t_rparen, 
        t_plus, t_iliteral, 
        t_newline,
        t_newline,
        t_eof
    };

    if (ARRAY_SIZE(expected) != lexer->size) return -1;
    for (i32_t i = 0; i < ARRAY_SIZE(expected); ++i) {
        if (lexer->tokens[i].type != expected[i]) return -1;
    }

    lexer_delete(lexer);
    return 0;
}

int main(void) {
    TRY(test_lexer_tokenize_string(), 0);
    TRY(test_lexer_tokenize_file(), 0);
    TEST_RESULT();
}
