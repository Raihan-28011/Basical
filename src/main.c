/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "main.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    lexer_t *lexer;
    parser_t *parser;
} globals_t;

globals_t g_vars;

void atexit_callback(void) {
    lexer_delete(g_vars.lexer);
    parser_delete(g_vars.parser);
    if (ast_print_buf) ast_print_buf_delete();
}

void usage(void) {
    fprintf(stderr, "usage: basical [FILENAME]\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    atexit(atexit_callback);
    if (argc < 2) usage();

    g_vars.lexer = lexer_new();
    lexer_tokenize_file(g_vars.lexer, argv[1]);
    if (g_vars.lexer->error_occured) {
        return 0;
    }
    lexer_print_tokens(g_vars.lexer);

    g_vars.parser = parser_new(g_vars.lexer);
    ast_module_t *m = parser_parse(g_vars.parser);
    if (m) {
        m->base.print((ast_node_t*)m);
        fprintf(stderr, "%s", ast_print_buf);
    }
    if (m) m->base.delete((ast_node_t*)m);
}
