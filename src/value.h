/*
 * created by: shunnobir
 * date: 18/12/2023
 */

#ifndef __BASICAL_VALUE_H__
#define __BASICAL_VALUE_H__

#include "ast.h"
#include "common.h"

typedef struct value value_t;
typedef struct value_int value_int_t;
typedef struct value_float value_float_t;
typedef struct value_list value_list_t;

typedef enum value_type {
    v_int,
    v_float,
    v_list,
} value_type_t;

struct value {
    value_type_t type;
    i32_t ref;      /* Reference count */
    void (*delete)(value_t *);
};

struct value_int {
    value_t base;
    i64_t   val;
};

struct value_float {
    value_t base;
    f64_t   val;
};

struct value_list {
    value_t base;
    value_t **elems;
    i32_t   cap;
    i32_t   len;
};

value_int_t *value_int_new(i64_t val);
value_float_t *value_float_new(f64_t val);
value_list_t *value_list_new(void);
value_list_t *value_list_push(value_list_t *list, value_t *v);

i64_t value_to_int(value_t *v);
f64_t value_to_float(value_t *v);
char *value_to_str(value_t *v);

void value_incref(value_t *v);
void value_decref(value_t *v);

#endif // __BASICAL_VALUE_H__
