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
    parser_t *_n        = (parser_t*)malloc(sizeof(parser_t));
    if (lexer) 
        _n->lexer       = lexer;
    _n->curtok          = 0;
    _n->tsize           = (lexer ? lexer->size : 0);
    _n->error_occured   = false;
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
                                                ast_node_inumber);
        return (ast_node_t*)ast_number_new((ast_number_internal_t){ .f = strtod(num.token, NULL) }, ast_node_fnumber);
    }

    return NULL;
}

static ast_node_t *parser_parse_unary(parser_t *parser) {
    if (parser_is_next(parser, (tokentype_t[1]){ t_lparen }, 1)) {
        // TODO: parser group statement
        return NULL;
    } else if (parser_is_next(parser, (tokentype_t[2]){ t_iliteral, t_fliteral }, 2)) {
        return parser_parse_num(parser);
    } else if (parser_is_next(parser, (tokentype_t[1]){ t_eof }, 1)) {
        return NULL;   
    } else {
        token_t tok = parser_nexttok(parser);
        em_parsing_error(EEEXP, tok.ln, tok.col);
        parser->error_occured = true;
        return NULL;
    }
}

static ast_node_t *parser_parse_factor(parser_t *parser) {
    ast_node_t *left = (ast_node_t*)parser_parse_unary(parser);
    ast_node_t *right = NULL;
    while (!parser->error_occured && parser_is_next(parser, (tokentype_t[4]){ t_star, t_slash, t_mod, t_pow }, 4)) {
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
    while (!parser->error_occured && parser_is_next(parser, (tokentype_t[2]){ t_plus, t_minus }, 2)) {
        tokentype_t op = parser_nexttok(parser).type;
        right = (ast_node_t*)parser_parse_factor(parser);
        left = (ast_node_t*)ast_term_new(left, right, ast_convert_toktype_to_astoptype(op));
    }

    return left;
}

static ast_node_t *parser_parse_expr(parser_t *parser) {
    ast_node_t *term = parser_parse_term(parser);
    ast_node_t *expr = (ast_node_t*)ast_expr_new(term);
    if (parser->error_occured) return NULL;
    return expr;
}

static ast_node_t *parser_parse_stmt(parser_t *parser) {
    ast_node_t *expr = parser_parse_expr(parser);
    ast_node_t *stmt = (ast_node_t*)ast_stmt_new(expr);
    if (!parser->error_occured && 
        !parser_match(parser, t_eof, 0) && 
        !parser_match(parser, t_newline, 0)) {
        token_t tok = parser_nexttok(parser);
        em_parsing_error(EENL, tok.ln, tok.col);
        parser->error_occured = true;
        return stmt;
    }
    parser_nexttok(parser);
    return stmt;
}

ast_module_t *parser_parse(parser_t *parser) {
    ast_module_t *module = ast_module_new();
    token_t tok = parser_peektok(parser, 0);
    em_set_fname(parser->lexer->fname);
    em_push_function("__global_main__", tok.ln, tok.col);
    while (!parser->error_occured && tok.type != t_eof) {
        ast_node_t *stmt = (ast_node_t*)parser_parse_stmt(parser);
        ast_module_insert(module, stmt);
        tok = parser_peektok(parser, 0);
    }
    em_pop_function();

    if (parser->error_occured && module) {
        module->base.delete((ast_node_t*)module);
        module = NULL;
    }

    return module;
}
