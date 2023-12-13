/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "error.h"
#include <stdio.h>
#include <stdarg.h>

char *error_type_to_str[] = {
    "could not open file",              // ECNOF
    "could not read file",              // ECNRF
    "expected expression",              // EEEXP
    "expected operator",                // EEOP
    "expected newline",                 // EENL
    "memory allocation failed",         // EMAF
    "expected closing parenthesis '('", // EECP
};

error_manager_t em = {
    .cssize = 0,
};

static void print_usr_msg(const char *fmt, va_list list) {
    fprintf(stderr, ": ");
    vfprintf(stderr, fmt, list);
}

void em_error(error_type_t type, char const *fmt, ...) {
    fprintf(stderr, "error: %s", error_type_to_str[type]);
    if (fmt) {
        va_list list;
        va_start(list, fmt);
        print_usr_msg(fmt, list);
        va_end(list);
    }
    fprintf(stderr, "\n");
}


void em_parsing_error(error_type_t type, i16_t line, i16_t col) {
    fprintf(stderr, "error: %s:%d:%d: at function '%s': %s\n", 
            em.fname, 
            line, 
            col, 
            em.call_stack[em.cssize-1].name, 
            error_type_to_str[type]);
    for (i16_t i = 0; i < em.cssize; ++i) {
        fprintf(stderr, "\tcalled from %s:%d:%d:%s()\n", 
                em.fname, 
                em.call_stack[i].ln, 
                em.call_stack[i].col, 
                em.call_stack[i].name); 
    }
}

void em_push_function(char *name, i16_t line, i16_t col) {
    if (em.cssize + 1 > MAX_CALL_STACK_LIMIT) {
        fprintf(stderr, "fatal: at function %s:%d:%d: call stack limit exceeded\n", 
                em.fname, 
                line, 
                col);
        return;
    }
    em.call_stack[em.cssize] = (call_chain_t) {
        .name = name,
        .ln   = line,
        .col  = col,
    };
    ++em.cssize;
}

void em_pop_function(void) {
    if (em.cssize - 1 < 0) {
        fprintf(stderr, "fatal: at function %s: call stack is already empty\n", em.fname);
        return; 
    }
    --em.cssize;
}

void em_set_fname(char const *fname) {
    em.fname = fname;
}
