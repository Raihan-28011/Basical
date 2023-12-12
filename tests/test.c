#include "test.h"
#include "../src/lexer.h"
#include "../src/parser.h"
#include <string.h>

#define ARRAY_SIZE(arr) (i32_t)(sizeof(arr)/sizeof(*arr))

tester_t g_tester = {
    .tests  = 0,
    .passed = 0,
    .errors = 0,
};

struct expectations {
    char lex_str[1024];
    tokentype_t expected[256];
    char ast_str_expected[1024];
} g_tests[] = {
    {
        .lex_str = "1212 + 2332.32 * 32 / 343 % (32 ** 323)",
        .expected = {
            t_iliteral, t_plus, t_fliteral,
            t_star, t_iliteral, t_slash,
            t_iliteral, t_mod, t_lparen,
            t_iliteral, t_pow, t_iliteral,
            t_rparen,
        },
        .ast_str_expected = "",
    },
};

static i32_t test_lexer_tokenize_string(void) {
    lexer_t *lexer = lexer_new();
    lexer_tokenize_string(lexer, "(132322 +4454545        ) \n\n\n* 3433", "repl");

    tokentype_t expected[] = {
        t_lparen, t_iliteral, t_plus, t_iliteral, t_rparen, 
        t_newline, 
        t_newline, 
        t_newline,
        t_star, t_iliteral, t_eof,
    };

    if (ARRAY_SIZE(expected) != lexer->size) {
        lexer_delete(lexer);        
        return -1;
    }
    for (i32_t i = 0; i < ARRAY_SIZE(expected); ++i) {
        if (lexer->tokens[i].type != expected[i]) {
            lexer_delete(lexer);
            return -1;
        }
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
        t_unrecognized, 
        t_lparen, t_fliteral,  t_rparen, 
        t_plus, t_iliteral, 
        t_newline,
        t_eof
    };

    if (ARRAY_SIZE(expected) != lexer->size) {
        lexer_delete(lexer);
        return -1;
    }
    for (i32_t i = 0; i < ARRAY_SIZE(expected); ++i) {
        if (lexer->tokens[i].type != expected[i]) {
            lexer_delete(lexer);
            return -1;
        }
    }

    lexer_delete(lexer);
    return 0;
}

static i32_t test_parser_parse(void) {
    lexer_t *lexer = lexer_new();
    for (i32_t i = 0; i < ARRAY_SIZE(g_tests); ++i) {
        lexer_tokenize_string(lexer, g_tests[i].lex_str, "repl");
        
        for (i32_t j = 0; j < lexer->size; ++j) {
            if (lexer->tokens[j].type != g_tests[i].expected[j]) {
                lexer_delete(lexer);
                return -1;
            }
        }

        parser_t *parser = parser_new(lexer);
        ast_module_t *module = parser_parse(parser);
        if (module) {
            module->base.print((ast_node_t*)module);
            if (strncmp(ast_print_buf, g_tests[i].ast_str_expected, a_blen) != 0) {
                lexer_delete(lexer);
                parser_delete(parser);
                return -1;
            }
            module->base.delete((ast_node_t*)module);
        }
        parser_delete(parser);
    }
    lexer_delete(lexer);
    ast_print_buf_delete();
    return 0;
}

int main(void) {
    TRY(test_lexer_tokenize_string(), 0);
    TRY(test_lexer_tokenize_file(), 0);
    TRY(test_parser_parse(), 0);
    TEST_RESULT();
}
