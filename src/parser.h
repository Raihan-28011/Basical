/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#ifndef __BASICAL_PARSER_H__
#define __BASICAL_PARSER_H__

#include "ast.h"
#include "lexer.h"
#include <stdbool.h>

typedef struct parser parser_t;
struct parser {
    i16_t         curtok;        // Current token position
    i16_t         tsize;         // No. of tokens
    bool          error_occured; // Has error occured
    lexer_t const *lexer;        // The lexer containing all the tokens
};

parser_t    *parser_new(lexer_t const *lexer);
void        parser_delete(parser_t *parser);
ast_module_t  *parser_parse(parser_t *parser);

#endif // __BASICAL_PARSER_H__
