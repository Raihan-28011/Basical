/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "common.h"
#include "error.h"
#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *lexer_read_file(char const *fname) {
    FILE *fd = fopen(fname, "rb");
    if (!fd) {
        em_lexical_error(et_could_not_open_file, "%s", fname);
        return NULL;
    }
    
    if (fseek(fd, 0, SEEK_END) == -1) {
        fclose(fd);
        em_lexical_error(et_could_not_read_file, "%s", fname);
        return NULL;
    }
    
    i64_t fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *content = (char*)malloc(sizeof(char) * (fsize+1));
    if ((i64_t)fread(content, sizeof(char), fsize, fd) != fsize) {
        free(content);
        fclose(fd);
        em_lexical_error(et_could_not_read_file, "%s", fname);
        return NULL;
    }
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
    if (lexer->pos >= lexer->slen) return '\0';
    ++lexer->col;
    return lexer->src[lexer->pos++];
}

inline static char lexer_peekc(lexer_t *lexer, i32_t offset) {
    if (lexer->pos + offset >= lexer->slen) return '\0';
    return lexer->src[lexer->pos + offset];
}

inline static token_t lexer_make_token(lexer_t *lexer, tokentype_t type, i16_t start, i16_t len) {
    token_t token = {
        .type = type,
        .len  = len,
        .ln   = lexer->line,
        .col  = lexer->col-len+1,
    };
    
    token.token = (char*)malloc(sizeof(char) * (len + 1));
    memcpy(token.token, lexer->src + start, len);
    token.token[len] = '\0';
    return token;
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
    char c      = lexer_peekc(lexer, 0);
    while (!lexer_iseof(c) && isspace(c)) {
        if (c == '\n') {
            lexer_push(lexer, lexer_make_token(lexer, t_newline, start, 1));
            ++lexer->line;
            lexer->col = 0;
        }
        lexer_nextc(lexer);
        c = lexer_peekc(lexer, 0);
    }
}

inline static void lexer_readnum(lexer_t *lexer) {
    char c = lexer_peekc(lexer, 0);
    while (!lexer_iseof(c) && isdigit(c)) { 
        lexer_nextc(lexer);
        c = lexer_peekc(lexer, 0); 
    }    
}

static void lexer_push_number(lexer_t *lexer) {
    tokentype_t type = t_iliteral;
    size_t start = lexer->pos;
    
    lexer_readnum(lexer);
    char c = lexer_peekc(lexer, 0);
    if (c == '.') {
        lexer_nextc(lexer);
        lexer_readnum(lexer);
        type = t_fliteral;
    }

    lexer_push(lexer, lexer_make_token(lexer, type, start, lexer->pos - start));
}

static void lexer_push_unrecognized(lexer_t *lexer) {
    size_t start = lexer->pos;
    char c = lexer_peekc(lexer, 0);
    while (!lexer_iseof(c) && !lexer_isdelim(c)) { 
        lexer_nextc(lexer);
        c = lexer_peekc(lexer, 0); 
    }
    lexer_push(lexer, lexer_make_token(lexer, t_unrecognised, start, lexer->pos - start));
}

void lexer_tokenize_string(lexer_t *lexer, const char *src, char const *fname) {
    lexer->fname = fname;
    lexer->src   = src;
    lexer->slen  = strlen(src);
    lexer->pos   = 0;
    char cur     = lexer_peekc(lexer, 0);
    while (!lexer_iseof(cur)) {
        lexer_skipws(lexer);
        cur = lexer_peekc(lexer, 0);
        switch (cur) {
        case '\0':
            break;
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
            if (lexer_peekc(lexer, 1) == '*') {
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
        cur = lexer_peekc(lexer, 0);
    }
    lexer_push(lexer, lexer_make_token(lexer, t_eof, lexer->pos, 0));
    lexer->src  = NULL;
    lexer->slen = 0;
    lexer->pos  = 0;
}

void lexer_tokenize_file(lexer_t *lexer, const char *fname) {
    char *src = lexer_read_file(fname);
    if (!src) {
        lexer->error_occured = true;
        return;
    }
    lexer_tokenize_string(lexer, src, fname);
    free(src);
    lexer->src = NULL;
    lexer->slen = 0;
    lexer->pos = 0;
}

token_t lexer_get_token(const lexer_t *lexer, i16_t index) {
    if (index >= lexer->size) return lexer->tokens[lexer->size-1];
    return lexer->tokens[index];
}

lexer_t *lexer_new(void) {
    lexer_t *_n         = (lexer_t*)malloc(sizeof(lexer_t));
    _n->size            = 0;
    _n->cap             = 1;
    _n->tokens          = NULL;
    _n->line            = 1;
    _n->col             = 0;
    _n->error_occured   = false;
    _n->src             = NULL;
    _n->pos             = 0;
    return _n;
}

void lexer_print_tokens(lexer_t *lexer) {
    for (i32_t i = 0; i < lexer->size; ++i) {
        token_print(&lexer->tokens[i]);
    }
}

void lexer_delete(lexer_t *lexer) {
    if (!lexer) return;
    if (lexer->src) free((void*)lexer->src);
    if (lexer->tokens) {
        for (i32_t i = 0; i < lexer->size; ++i) 
            if (lexer->tokens[i].token) free(lexer->tokens[i].token);
        free(lexer->tokens);
    }
    if (lexer) free(lexer);
}
