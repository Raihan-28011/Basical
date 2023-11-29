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
    FILE *fd = fopen(fname, "r");
    if (!fd) error("could not open file: %s", fname);
    
    if (fseek(fd, 0, SEEK_END) == -1) error("could not read file: %s", fname);
    
    i64_t fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *content = (char*)malloc(sizeof(char) * fsize);
    if ((i64_t)fread(content, sizeof(char), fsize, fd) != fsize) 
        error("could not read file: %s", fname);
    content[fsize] = '\0';
    fclose(fd);
    return content;
}

static bool lexer_iseof(char c) {
    return c == '\0';
}

static bool lexer_isdelim(char c) {
    return isspace(c) || c == '\0' || c == '(' || c == ')'; 
}

static void lexer_resize(lexer_t *lexer) {
    if (lexer->size + 1 >= lexer->cap) {
        lexer->cap    = lexer->cap * LEXER_GROW_FACTOR;
        lexer->tokens = (token_t*)realloc(lexer->tokens, sizeof(token_t) * lexer->cap);
    }
}

static void lexer_push(lexer_t *lexer, token_t token) {
    lexer_resize(lexer);
    lexer->tokens[lexer->size++] = token;
}

static void lexer_skipws(lexer_t *lexer, char const *src, size_t *pos) {
    char c = src[*pos];
    while (!lexer_iseof(c) && isspace(c)) {
        if (c == '\n') {
            lexer_push(lexer, (token_t){ t_newline, "\n", 1, lexer->line, (i16_t)(*pos+1) });
            ++lexer->line;
        }
        ++(*pos);
        c = src[*pos];
    }
}

static char lexer_peek(char const *src, size_t *pos) {
    return src[(*pos)+1];
}

static void lexer_push_number(lexer_t *lexer, char const *src, size_t *pos) {
    tokentype_t type = t_iliteral;
    size_t start = *pos;
    char c = src[*pos];
    while (!lexer_iseof(c) && isdigit(c)) {
        ++(*pos);
        c = src[*pos];
    }
    if (c == '.') {
        ++(*pos);
        while (!lexer_iseof(c) && isdigit(c)) {
            ++(*pos);
            c = src[*pos];
        }
        type = t_fliteral;
    }

    lexer_push(lexer, (token_t){ type, src + start, *pos - start, lexer->line, start });
    --(*pos);
}

static void lexer_push_unrecognized(lexer_t *lexer, char const *src, size_t *pos) {
    size_t start = *pos;
    char c = src[*pos];
    while (!lexer_iseof(c) && !lexer_isdelim(c)) {
        ++(*pos);
        c = src[*pos];
    }
    lexer_push(lexer, (token_t){ t_unrecognised, src + start, *pos - start, lexer->line, start });
    --(*pos);
}

void lexer_tokenize_file(lexer_t *lexer, const char *fname) {
    char   *src = lexer_read_file(fname);
    lexer->src  = src;
    size_t pos  = 0;

    char cur    = src[pos];
    while (!lexer_iseof(cur)) {
        lexer_skipws(lexer, src, &pos);
        cur     = src[pos];
        switch (cur) {
        case '+':
            lexer_push(lexer, (token_t){ t_plus, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case '-':
            lexer_push(lexer, (token_t){ t_minus, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case '/':
            lexer_push(lexer, (token_t){ t_slash, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case '*':
            if (lexer_peek(src, &pos) == '*') {
                lexer_push(lexer, (token_t){ t_pow, src + pos, 2, lexer->line, (i16_t)(pos+1) });
                ++pos;
            } else
                lexer_push(lexer, (token_t){ t_star, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case '%':
            lexer_push(lexer, (token_t){ t_mod, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case '(':
            lexer_push(lexer, (token_t){ t_lparen, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        case ')':
            lexer_push(lexer, (token_t){ t_rparen, src + pos, 1, lexer->line, (i16_t)(pos+1) });
            break;
        default:
            if (isdigit(cur)) lexer_push_number(lexer, src, &pos);
            else lexer_push_unrecognized(lexer, src, &pos);
            break;
        }
        ++pos;
        cur = src[pos];
    }
    lexer_push(lexer, (token_t){ t_eof, src + pos, 1, lexer->line, (i16_t)(pos+1) });
}

lexer_t *lexer_new(void) {
    lexer_t *_n = (lexer_t*)malloc(sizeof(lexer_t));
    _n->size    = 0;
    _n->cap     = 1;
    _n->tokens  = NULL;
    _n->line    = 1;
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
