/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#ifndef __BASICAL_ERROR_H__
#define __BASICAL_ERROR_H__

#include "common.h"
#define MAX_CALL_STACK_LIMIT 10

typedef struct error_manager error_manager_t;
typedef struct call_chain call_chain_t;

struct call_chain {
    char *name;
    i16_t ln, col;
};

typedef enum error_type {
    ECNOF,
    ECNRF,
    EEEXP,
    EEOP,
    EENL,
    EMAF,
} error_type_t;

struct error_manager {
    call_chain_t call_stack[MAX_CALL_STACK_LIMIT];
    char const *fname;
    i16_t cssize;
};

void em_error(error_type_t type, char const *fmt, ...);
void em_parsing_error(error_type_t type, i16_t line, i16_t col);
void em_push_function(char *name, i16_t line, i16_t col);
void em_pop_function(void);
void em_set_fname(char const *fname);

#endif // __BASICAL_ERROR_H__
