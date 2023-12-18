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

struct {
    char const *fname;
    tokentype_t lex_expected[100];
    char *ast_expected;
} g_tests[] = {
    {
        .fname = "../tests/tokenize.math",
        .lex_expected = {
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
            t_eof,
        },
        .ast_expected = "",
    },
    {
        .fname = "../tests/expr1.math",
        .lex_expected = {
            t_iliteral, t_star, t_iliteral, t_newline, 
            t_iliteral, t_plus, t_iliteral, t_newline, 
            t_eof, 
        },
        .ast_expected = 
            "(package"
            "    (stmt"
            "        (expr"
            "            (factor"
            "                232"
            "                332"
            "                *"
            "            factor)"
            "        expr)"
            "    stmt)"
            "    (stmt"
            "        (expr"
            "            (term"
            "                54"
            "                545"
            "                +"
            "            term)"
            "        expr)"
            "    stmt)"
            "package)",
    },
    {
        .fname = "../tests/expr2.math",
        .lex_expected = {
            t_iliteral, t_star, t_fliteral, 
            t_plus, t_iliteral, t_mod, 
            t_iliteral, t_slash, t_iliteral, 
            t_pow, t_iliteral, t_newline, 
            t_eof, 
        },
        .ast_expected = 
            "(package"
            "    (stmt"
            "        (expr"
            "            (factor"
            "                232"
            "                332"
            "                *"
            "            factor)"
            "        expr)"
            "    stmt)"
            "    (stmt"
            "        (expr"
            "            (term"
            "                54"
            "                545"
            "                +"
            "            term)"
            "        expr)"
            "    stmt)"
            "package)",
    },
    {
        .fname = "../tests/expr3.math",
        .lex_expected = {
            t_iliteral, t_plus, t_newline, 
            t_eof,
        },
        .ast_expected = "",
    },
    {
        .fname = "../tests/expr4.math",
        .lex_expected = {
            t_iliteral, t_plus, t_fliteral, 
            t_star, t_iliteral, t_slash, 
            t_iliteral, t_mod, t_lparen, 
            t_iliteral, t_newline, t_pow, 
            t_newline, t_iliteral, t_newline, 
            t_rparen, t_newline, t_eof,
        },
        .ast_expected = 
            "(package"
            "    (stmt"
            "        (expr"
            "            (term"
            "                1212"
            "                (factor"
            "                    (factor"
            "                        (factor"
            "                            2332.320000"
            "                            32"
            "                            *"
            "                        factor)"
            "                        343"
            "                        /"
            "                    factor)"
            "                    (expr"
            "                        (factor"
            "                            32"
            "                            323"
            "                            **"
            "                        factor)"
            "                    expr)"
            "                    %"
            "                factor)"
            "                +"
            "            term)"
            "        expr)"
            "    stmt)"
            "package)",
    },
    {
        .fname = "../tests/expr5.math",
        .lex_expected = {
            t_lparen, t_iliteral, t_plus, 
            t_fliteral, t_star, t_iliteral, 
            t_slash, t_iliteral, t_mod, 
            t_iliteral, t_pow, t_fliteral, 
            t_newline, t_newline, t_newline, 
            t_newline, t_newline, t_rparen, 
            t_newline, t_eof,
        },
        .ast_expected = 
            "(package"
            "    (stmt"
            "        (expr"
            "            (expr"
            "                (term"
            "                    1212"
            "                    (factor"
            "                        (factor"
            "                            (factor"
            "                                (factor"
            "                                    2332.320000"
            "                                    32"
            "                                    *"
            "                                factor)"
            "                                343"
            "                                /"
            "                            factor)"
            "                            32"
            "                            %"
            "                        factor)"
            "                        323.320000"
            "                        **"
            "                    factor)"
            "                    +"
            "                term)"
            "            expr)"
            "        expr)"
            "    stmt)"
            "package)",
    },
    {
        .fname = "../tests/expr6.math",
        .lex_expected = {
            t_lparen, t_newline, 
            t_newline, 
            t_newline, 
            t_newline, 
            t_iliteral, t_newline, 
            t_newline, 
            t_rparen, t_newline, 
            t_eof, 
        },
        .ast_expected = 
            "(package"
            "    (stmt"
            "        (expr"
            "            (expr"
            "                23"
            "            expr)"
            "        expr)"
            "    stmt)"
            "package)",
    },
    {
        .fname = "../tests/expr7.math",
        .lex_expected = {
            t_iliteral, t_plus, t_fliteral, t_fliteral, t_newline, 
            t_eof,
        },
        .ast_expected = "",
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

    bool failed = false;
    for (i32_t i = 0; i < ARRAY_SIZE(g_tests); ++i) {
        lexer_t *lexer = lexer_new();
        lexer_tokenize_file(lexer, g_tests[i].fname);

        for (i32_t j = 0; j < lexer->tcount; ++j) {
            if (lexer->tokens[j].type != g_tests[i].lex_expected[j]) {
                fprintf(stderr, "(Test #%d:#%d) " TEST_FAILED "\n", g_tester.tests, i+1);
                failed = true;
                continue;
            }
        }

        fprintf(stderr, "(Test #%d:#%d) " TEST_PASSED "\n", g_tester.tests, i+1);
        lexer_delete(lexer);
    }
    return failed;
}

static i32_t test_parser_parse(void) {
    bool failed = false;
    for (i32_t i = 0; i < ARRAY_SIZE(g_tests); ++i) {
        lexer_t *lexer = lexer_new();
        lexer_tokenize_file(lexer, g_tests[i].fname);
        parser_t *parser = parser_new(lexer);
        ast_package_t *package = parser_parse(parser);
        if (package) {
            if (ast_print_buf && 
                strncmp(ast_print_buf, g_tests[i].ast_expected, a_blen) != 0) {
                fprintf(stderr, "(Test #%d:#%d) " TEST_FAILED "\n", g_tester.tests, i+1);
                failed = true;
                continue;
            }
            package->base.print((ast_node_t*)package);
            package->base.delete((ast_node_t*)package);
        }        
        fprintf(stderr, "(Test #%d:#%d) " TEST_PASSED "\n", g_tester.tests, i+1);
        lexer_delete(lexer);
        parser_delete(parser);
    }
    ast_print_buf_delete();
    return failed;
}

int main(void) {
    TRY(test_lexer_tokenize_string(), 0);
    TRY(test_lexer_tokenize_file(), 0);
    TRY(test_parser_parse(), 0);
    TEST_RESULT();
}
