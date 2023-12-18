/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "main.h"
#include "ast.h"
#include "evaluate.h"
#include "lexer.h"
#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    lexer_t *lexer;
    parser_t *parser;
} globals_t;

globals_t g_vars;

void atexit_callback(void) {
    if (g_vars.lexer) lexer_delete(g_vars.lexer);
    if (g_vars.parser) parser_delete(g_vars.parser);
    if (ast_print_buf) ast_print_buf_delete();
    stack_delete();
}

void usage(void) {
    fprintf(stderr, "usage: basical [FILENAME]\n");
    exit(1);
}

static char *trim(char *str, i32_t *len) {
    char *start = str,
         *end   = str + (*len) - 1;
    while (isspace(*start))
        ++start, --*len;
    while (isspace(*end)) 
        --end, --*len;
    *(end+1) = '\0';
    return start;
}

int repl(void) {
#define MAX_BUF_SIZE 1024
    char buf[MAX_BUF_SIZE+1];
    while (true) {
        if (g_vars.lexer) {
            lexer_delete(g_vars.lexer);
            g_vars.lexer = NULL;
        }

        if (g_vars.parser) {
            parser_delete(g_vars.parser);
            g_vars.parser = NULL;
        }

        /* print the prompt */
        fprintf(stdout, TEXT_BOLD(TEXT_BLUE("-> ")));

        i32_t i = 0;
        char c;
        
        /* filter out the leading space characters from user input */
        while (isspace(c = fgetc(stdin)) && c != '\n');
        
        /* if it's newline then reprompt the user */
        if (c == '\n') continue;

        /* return the character if it's not a space character */
        ungetc(c, stdin);

        /* read until user inserts semicolon or the buffer is filled to the max size */
        while ((c = fgetc(stdin)) != ';' && c != EOF && i <= MAX_BUF_SIZE)
            buf[i++] = c;
        buf[i] = '\0';

        /* discard the trailing inputs */
        if (i == MAX_BUF_SIZE || c == ';') {
            while ((c = fgetc(stdin)) != '\n' && c != EOF);
        }

        /* trim the buffer */
        char *code = trim(buf, &i);

        /* early check if user wants to quit or has entered EOF character */
        if (c == EOF || 
            (i == 4 && strncmp(code, "quit", 4) == 0)) break;
        
        g_vars.lexer = lexer_new();
        lexer_tokenize_string(g_vars.lexer, code, "repl");
        g_vars.parser = parser_new(g_vars.lexer);
        ast_package_t *package = parser_parse(g_vars.parser);

#ifdef BASICAL_DEBUG
        if (package) {
            package->base.print((ast_node_t*)package);
            package->base.evaluate((ast_node_t*)package);
            fprintf(stderr, "%s", ast_print_buf);
        }
#endif

        if (package) package->base.delete((ast_node_t*)package); 
    }
    return 0;
}

int main(int argc, char *argv[]) {
    atexit(atexit_callback);
    if (argc < 2) {
        return repl();
    }

    g_vars.lexer = lexer_new();
    lexer_tokenize_file(g_vars.lexer, argv[1]);
    if (g_vars.lexer->error_occured) {
        return -1;
    }

#ifdef BASICAL_DEBUG
    lexer_print_tokens(g_vars.lexer);
#endif

    g_vars.parser = parser_new(g_vars.lexer);
    ast_package_t *m = parser_parse(g_vars.parser);

#ifdef BASICAL_DEBUG
    if (m) {
        m->base.print((ast_node_t*)m);
        m->base.evaluate((ast_node_t*)m);
        fprintf(stderr, "%s", ast_print_buf);
    }
#endif

    if (m) m->base.delete((ast_node_t*)m);
}
