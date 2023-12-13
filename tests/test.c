#include "test.h"
#include "../src/lexer.h"
#include "../src/parser.h"
#include <stdio.h>
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
        .lex_str = "1212 + 2332.32 * 32 / 343 % (32 ** 323\t\t\t)\n",
        .expected = {
            t_iliteral, t_plus, t_fliteral,
            t_star, t_iliteral, t_slash,
            t_iliteral, t_mod, t_lparen,
            t_iliteral, t_pow, t_iliteral,
            t_rparen, t_newline, t_eof,
        },
        .ast_str_expected =
            "(module\n"
            "    (stmt\n"
            "        (expr\n"
            "            (term\n"
            "                1212\n"
            "                (factor\n"
            "                    (factor\n"
            "                        (factor\n"
            "                            2332.320000\n"
            "                            32\n"
            "                            *\n"
            "                        factor)\n"
            "                        343\n"
            "                        /\n"
            "                    factor)\n"
            "                    (expr\n"
            "                        (factor\n"
            "                            32\n"
            "                            323\n"
            "                            **\n"
            "                        factor)\n"
            "                    expr)\n"
            "                    %\n"
            "                factor)\n"
            "                +\n"
            "            term)\n"
            "        expr)\n"
            "    stmt)\n"
            "module)\n",
    },
    {
        .lex_str = "(\n",
        .expected = {
            t_lparen, t_newline, t_eof,
        },
        .ast_str_expected = "",
    },
    {
        .lex_str = "()\n",
        .expected = {
            t_lparen, t_rparen, t_newline, t_eof,
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

    if (ARRAY_SIZE(expected) != lexer->tcount) {
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

    if (ARRAY_SIZE(expected) != lexer->tcount) {
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
    bool failed = false;
    for (i32_t i = 0; i < ARRAY_SIZE(g_tests); ++i) {
        lexer_tokenize_string(lexer, g_tests[i].lex_str, "repl");
        
        for (i32_t j = 0; j < lexer->tcount; ++j) {
            if (lexer->tokens[j].type != g_tests[i].expected[j]) {
                fprintf(stderr, "(Test #%d:#%d) " TEST_FAILED "\n", g_tester.tests, i+1);
                failed = true;
                continue;
            }
        }

        parser_t *parser = parser_new(lexer);
        ast_module_t *module = parser_parse(parser);
        if (module) {
            if (strncmp(ast_print_buf, g_tests[i].ast_str_expected, a_blen) != 0) {
                fprintf(stderr, "(Test #%d:#%d) " TEST_FAILED "\n", g_tester.tests, i+1);
                failed = true;
                continue;
            }
            module->base.print((ast_node_t*)module);
            module->base.delete((ast_node_t*)module);
            fprintf(stderr, "(Test #%d:#%d) " TEST_PASSED "\n", g_tester.tests, i+1);
        } else {
            fprintf(stderr, "(Test #%d:#%d) " TEST_FAILED "\n", g_tester.tests, i+1);
            failed = true; 
        }
        parser_delete(parser);
    }
    lexer_delete(lexer);
    ast_print_buf_delete();
    return failed;
}

int main(void) {
    TRY(test_lexer_tokenize_string(), 0);
    TRY(test_lexer_tokenize_file(), 0);
    TRY(test_parser_parse(), 0);
    TEST_RESULT();
}
