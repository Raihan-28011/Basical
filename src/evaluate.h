/*
 * created by: shunnobir
 * date: 18/12/2023
 */

#ifndef __BASICAL_EVALUATE_H__
#define __BASICAL_EVALUATE_H__

#include "common.h"
#include "value.h"

typedef struct stack_t stack_t;
struct stack_t {
    value_t **mem;
    i32_t top;
    i32_t cap;
};

void evaluate_add(void);
void evaluate_sub(void);
void evaluate_mul(void);
void evaluate_div(void);
void evaluate_mod(void);
void evaluate_pow(void);
void evaluate_int(i64_t i);
void evaluate_float(f64_t f);

void stack_delete(void);

#endif // __BASICAL_EVALUATE_H__
