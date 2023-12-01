/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "common.h"
#include "error.h"
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

static char *lexer_read_file(char const *fname) {
    FILE *fd = fopen(fname, "rb");
    if (!fd) error("could not open file: %s", fname);
    
    if (fseek(fd, 0, SEEK_END) == -1) error("could not read file: %s", fname);
    
    i64_t fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *content = (char*)malloc(sizeof(char) * (fsize+1));
    if ((i64_t)fread(content, sizeof(char), fsize, fd) != fsize) 
        error("could not read file: %s", fname);
    content[fsize] = '\0';
    fclose(fd);
    return content;
}

inline static bool lexer_iseof(char c) {
    return c == '\0';
}

inline static bool lexer_isdelim(char c) {
    return isspace(c) || c == '\0' || c == '\n' || c == '(' || c == ')'; 
}

inline static char lexer_nextc(lexer_t *lexer) {
    return lexer->src[lexer->pos++];
}

inline static char lexer_peekc(lexer_t *lexer) {
    return lexer->src[lexer->pos];
}

inline static token_t lexer_make_token(lexer_t *lexer, tokentype_t type, i16_t start, i16_t len) {
    return (token_t){ type, lexer->src + start, len, lexer->line, start+1 };
}

inline static void lexer_resize(lexer_t *lexer) {
    if (lexer->size + 1 >= lexer->cap) {
        lexer->cap    = lexer->cap * LEXER_GROW_FACTOR;
        lexer->tokens = (token_t*)realloc(lexer->tokens, sizeof(token_t) * lexer->cap);
    }
}

inline static void lexer_push(lexer_t *lexer, token_t token) {
    lexer_resize(lexer);
    lexer->tokens[lexer->size++] = token;
}

static void lexer_skipws(lexer_t *lexer) {
    i16_t start = lexer->pos;
    char c      = lexer_peekc(lexer);
    while (!lexer_iseof(c) && isspace(c)) {
        if (c == '\n') {
            lexer_push(lexer, lexer_make_token(lexer, t_newline, start, 1));
            ++lexer->line;
        }
        lexer_nextc(lexer);
        c = lexer_peekc(lexer);
    }
}

inline static void lexer_readnum(lexer_t *lexer) {
    char c = lexer_peekc(lexer);
    while (!lexer_iseof(c) && isdigit(c)) { 
        lexer_nextc(lexer);
        c = lexer_peekc(lexer); 
    }    
}

static void lexer_push_number(lexer_t *lexer) {
    tokentype_t type = t_iliteral;
    size_t start = lexer->pos;
    
    lexer_readnum(lexer);
    char c = lexer_peekc(lexer);
    if (c == '.') {
        lexer_nextc(lexer);
        lexer_readnum(lexer);
        type = t_fliteral;
    }

    lexer_push(lexer, lexer_make_token(lexer, type, start, lexer->pos - start));
}

static void lexer_push_unrecognized(lexer_t *lexer) {
    size_t start = lexer->pos;
    char c = lexer_peekc(lexer);
    while (!lexer_iseof(c) && !lexer_isdelim(c)) { 
        lexer_nextc(lexer);
        c = lexer_peekc(lexer); 
    }
    lexer_push(lexer, lexer_make_token(lexer, t_unrecognised, start, lexer->pos - start));
}

void lexer_tokenize_file(lexer_t *lexer, const char *fname) {
    char   *src = lexer_read_file(fname);
    lexer->src  = src;
    lexer->pos  = 0;

    char cur    = lexer_peekc(lexer);
    while (!lexer_iseof(cur)) {
        lexer_skipws(lexer);
        cur = lexer_peekc(lexer);
        switch (cur) {
        case '+':
            lexer_push(lexer, lexer_make_token(lexer, t_plus, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        case '-':
            lexer_push(lexer, lexer_make_token(lexer, t_minus, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        case '/':
            lexer_push(lexer, lexer_make_token(lexer, t_slash, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        case '*':
            if (lexer_peekc(lexer) == '*') {
                lexer_push(lexer, lexer_make_token(lexer, t_pow, lexer->pos, 1));
                lexer_nextc(lexer);
            } else {
                lexer_push(lexer, lexer_make_token(lexer, t_star, lexer->pos, 1));
            }
            lexer_nextc(lexer);
            break;
        case '%':
            lexer_push(lexer, lexer_make_token(lexer, t_mod, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        case '(':
            lexer_push(lexer, lexer_make_token(lexer, t_lparen, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        case ')':
            lexer_push(lexer, lexer_make_token(lexer, t_rparen, lexer->pos, 1));
            lexer_nextc(lexer);
            break;
        default:
            if (isdigit(cur)) lexer_push_number(lexer);
            else lexer_push_unrecognized(lexer);
            break;
        }
        cur = lexer_peekc(lexer);
    }
    lexer_push(lexer, lexer_make_token(lexer, t_eof, lexer->pos, 0));
}

lexer_t *lexer_new(void) {
    lexer_t *_n = (lexer_t*)malloc(sizeof(lexer_t));
    _n->size    = 0;
    _n->cap     = 1;
    _n->tokens  = NULL;
    _n->line    = 1;
    _n->src     = NULL;
    _n->pos     = 0;
    return _n;
}

void lexer_print_tokens(lexer_t *lexer) {
    for (i32_t i = 0; i < lexer->size; ++i) {
        token_print(&lexer->tokens[i]);
    }
}

void lexer_delete(lexer_t *lexer) {
    free(lexer->src);
    free(lexer->tokens);
    free(lexer);
}
