/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "common.h"
#include "error.h"
#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t *lexer_new(void) {
    lexer_t *_n         = (lexer_t*)malloc(sizeof(lexer_t));
    if (!_n) {
        em_error(EMAF, "%s", strerror(errno));
        return NULL;
    }

    *_n                 = (lexer_t){
        .tcount         = 0,
        .cap            = 1,
        .tokens         = NULL,
        .line           = 1,
        .col            = 0,
        .error_occured  = false,
        .src            = NULL,
        .pos            = 0,
    };
    return _n;
}

void lexer_delete(lexer_t *lexer) {
    if (!lexer) return;
    if (lexer->tokens) {
        for (i32_t i = 0; i < lexer->tcount; ++i)
            if (lexer->tokens[i].token) free(lexer->tokens[i].token);
        free(lexer->tokens);
    }
    free(lexer);
}

static char *lexer_read_file(char const *fname) {
    FILE *fd = fopen(fname, "rb");
    if (!fd) {
        em_error(ECNOF, "'%s': %s", fname, strerror(errno));
        return NULL;
    }

    if (fseek(fd, 0, SEEK_END) == -1) {
        fclose(fd);
        em_error(ECNRF, "'%s': %s", fname, strerror(errno));
        return NULL;
    }

    i64_t fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *buf = (char*)malloc(sizeof(char) * (fsize+1));
    if (!buf) {
        fclose(fd);
        em_error(EMAF, "%s", strerror(errno));
        return NULL;
    }

    if ((i64_t)fread(buf, sizeof(char), fsize, fd) != fsize) {
        free(buf);
        fclose(fd);
        em_error(ECNRF, "'%s'", fname);
        return NULL;
    }
    buf[fsize] = '\0';
    fclose(fd);
    return buf;
}

inline static bool lexer_isdelim(char c) {
    return isspace(c) || c == '\0' || c == '(' || c == ')';
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

inline static token_t lexer_make_token(lexer_t *lexer, tokentype_t type, i16_t curpos, i16_t len) {
    token_t token = {
        .type     = type,
        .len      = len,
        .ln       = lexer->line,
        .col      = lexer->col-len+1,
        .token    = (char*)malloc(sizeof(char) * (len + 1)),
    };

    if (!token.token) {
        em_error(EMAF, "%s", strerror(errno));
        lexer->error_occured = true;
        return (token_t){ .type = t_eof };
    }
    
    memcpy(token.token, lexer->src + curpos - len, len);
    token.token[len] = '\0';
    return token;
}

inline static void lexer_resize(lexer_t *lexer) {
    if (lexer->tcount + 1 >= lexer->cap) {
        lexer->cap    = lexer->cap * LEXER_GROW_FACTOR;
        lexer->tokens = (token_t*)realloc(lexer->tokens, sizeof(token_t) * lexer->cap);
        if (!lexer->tokens) {
            em_error(EMAF, "%s", strerror(errno));
            lexer->error_occured = true;
        }
    }
}

inline static void lexer_push(lexer_t *lexer, token_t token) {
    lexer_resize(lexer);
    if (lexer->error_occured) return;
    lexer->tokens[lexer->tcount++] = token;
}

static void lexer_skipws(lexer_t *lexer) {
    char c;
    while ((c = lexer_peekc(lexer, 0)) && isspace(c)) {
        if (c == '\n') {
            lexer_push(lexer, lexer_make_token(lexer, t_newline, lexer->pos, 1));
            ++lexer->line;
            lexer->col = 0;
        }
        lexer_nextc(lexer);
    }
}

inline static void lexer_readnum(lexer_t *lexer) {
    char c;
    while ((c = lexer_peekc(lexer, 0)) && isdigit(c))
        lexer_nextc(lexer);
}

static void lexer_push_number(lexer_t *lexer) {
    tokentype_t type = t_iliteral;
    size_t start = lexer->pos;

    lexer_readnum(lexer);
    if (lexer_peekc(lexer, 0)  == '.') {
        lexer_nextc(lexer);
        lexer_readnum(lexer);
        type = t_fliteral;
    }

    lexer_push(lexer, lexer_make_token(lexer, type, lexer->pos, lexer->pos - start));
}

static void lexer_push_unrecognized(lexer_t *lexer) {
    size_t start = lexer->pos;
    char c;
    while ((c = lexer_peekc(lexer, 0)) && !lexer_isdelim(c))
        lexer_nextc(lexer);
    lexer_push(lexer, lexer_make_token(lexer, t_unrecognized, lexer->pos, lexer->pos - start));
}

void lexer_tokenize_string(lexer_t *lexer, const char *src, char const *fname) {
    lexer->fname = fname;
    lexer->src   = src;
    lexer->slen  = strlen(src);
    char cur;
    while ((cur = lexer_peekc(lexer, 0))) {
        lexer_skipws(lexer);
        cur = lexer_peekc(lexer, 0);
        switch (cur) {
        case '\0':
            break;
        case '+':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_plus, lexer->pos, 1));
            break;
        case '-':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_minus, lexer->pos, 1));
            break;
        case '/':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_slash, lexer->pos, 1));
            break;
        case '*':
             if (lexer_peekc(lexer, 1) == '*') {
                 lexer_nextc(lexer);
                 lexer_nextc(lexer);
                 lexer_push(lexer, lexer_make_token(lexer, t_pow, lexer->pos, 1));
             } else {
                 lexer_nextc(lexer);
                 lexer_push(lexer, lexer_make_token(lexer, t_star, lexer->pos, 1));
            }
            break;
        case '%':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_mod, lexer->pos, 1));
            break;
        case '(':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_lparen, lexer->pos, 1));
            break;
        case ')':
            lexer_nextc(lexer);
            lexer_push(lexer, lexer_make_token(lexer, t_rparen, lexer->pos, 1));
            break;
        default:
            if (isdigit(cur)) lexer_push_number(lexer);
            else lexer_push_unrecognized(lexer);
            break;
        }

        if (lexer->error_occured) break;
    }
    lexer_push(lexer, lexer_make_token(lexer, t_eof, lexer->pos, 0));
}

void lexer_tokenize_file(lexer_t *lexer, const char *fname) {
    char *src = lexer_read_file(fname);
    if (!src) {
        lexer->error_occured = true;
        return;
    }
    lexer_tokenize_string(lexer, src, fname);
    free(src);
}

token_t lexer_get_token(const lexer_t *lexer, i16_t index) {
    if (index >= lexer->tcount) return lexer->tokens[lexer->tcount-1];
    return lexer->tokens[index];
}

void lexer_print_tokens(lexer_t *lexer) {
    for (i32_t i = 0; i < lexer->tcount; ++i) {
        token_print(&lexer->tokens[i]);
    }
}
