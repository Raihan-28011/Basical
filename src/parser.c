/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#include "parser.h"
#include "ast.h"
#include "error.h"
#include "token.h"
#include <stdbool.h>
#include <stdlib.h>

parser_t *parser_new(lexer_t const *lexer) {
    parser_t *_n  = (parser_t*)malloc(sizeof(parser_t));
    if (lexer) 
        _n->lexer = lexer;
    _n->curtok    = 0;
    _n->tsize     = (lexer ? lexer->size : 0);
    return _n;
}

void parser_delete(parser_t *parser) {
    if (!parser) return;
    if (parser) free(parser);
}

inline static token_t parser_nexttok(parser_t *parser) {
    return lexer_get_token(parser->lexer, parser->curtok++);
}

inline static token_t parser_peektok(parser_t *parser, i16_t offset) {
    return lexer_get_token(parser->lexer, parser->curtok + offset);
}

inline static bool parser_match(parser_t *parser, tokentype_t type, i16_t offset) {
    return (parser_peektok(parser, offset).type == type);
}

inline static bool parser_is_next(parser_t *parser, tokentype_t *types, i16_t size) {
    for (i16_t i = 0; i < size; ++i) {
        if (parser_match(parser, types[i], 0)) {
            return true;
        }
    }
    
    return false;
}

static ast_node_t *parser_parse_expr(parser_t *parser);

static ast_node_t *parser_parse_num(parser_t *parser) {
    if (parser_is_next(parser, (tokentype_t[2]){ t_iliteral, t_fliteral }, 2)) {
        token_t num = parser_nexttok(parser);
        if (num.type == t_iliteral) 
            return (ast_node_t*)ast_number_new((ast_number_internal_t) { .i = strtoll(num.token, NULL, 10) }, 
                                                AST_INUMBER);
        return (ast_node_t*)ast_number_new((ast_number_internal_t){ .f = strtod(num.token, NULL) }, AST_FNUMBER);
    }

    return NULL;
}

static ast_node_t *parser_parse_unary(parser_t *parser) {
    if (parser_is_next(parser, (tokentype_t[1]){ t_lparen }, 1)) {
        // TODO: parser group statement
        return NULL;
    } else {
        return parser_parse_num(parser);
    }
}

static ast_node_t *parser_parse_factor(parser_t *parser) {
    ast_node_t *left = (ast_node_t*)parser_parse_unary(parser);
    ast_node_t *right = NULL;
        while (parser_is_next(parser, (tokentype_t[4]){ t_star, t_slash, t_mod, t_pow }, 4)) {
            tokentype_t op = parser_nexttok(parser).type;
            right = (ast_node_t*)parser_parse_unary(parser);
            left  = (ast_node_t*)ast_factor_new(left, right,
                                ast_convert_toktype_to_astoptype(op));
        }
    return left;
}

static ast_node_t *parser_parse_term(parser_t *parser) {
    ast_node_t *left = (ast_node_t*)parser_parse_factor(parser); 
    ast_node_t *right = NULL;
    while (parser_is_next(parser, (tokentype_t[2]){ t_plus, t_minus }, 2)) {
        tokentype_t op = parser_nexttok(parser).type;
        right = (ast_node_t*)parser_parse_factor(parser);
        left = (ast_node_t*)ast_term_new(left, right, ast_convert_toktype_to_astoptype(op));
    }
    return left;
}

static ast_node_t *parser_parse_expr(parser_t *parser) {
    ast_node_t *term = parser_parse_term(parser);
    ast_node_t *expr = (ast_node_t*)ast_expr_new(term);
    return expr;
}

static ast_node_t *parser_parse_stmt(parser_t *parser) {
    ast_node_t *expr = parser_parse_expr(parser);
    ast_node_t *stmt = (ast_node_t*)ast_stmt_new(expr);
    if (!parser_match(parser, t_newline, 0)) {
        token_t tok = parser_nexttok(parser);
        error("%s:%d:%d expected new line", parser->lexer->fname, tok.ln, tok.col);
    }
    parser_nexttok(parser);
    return stmt;
}

ast_main_t *parser_parse(parser_t *parser) {
    ast_main_t *main = ast_main_new();
    token_t tok = parser_peektok(parser, 0);
    while (tok.type != t_eof) {
        ast_node_t *stmt = (ast_node_t*)parser_parse_stmt(parser);
        ast_main_insert(main, stmt);
        tok = parser_peektok(parser, 0);
    }

    return main;
}
