/*
 * created by: shunnobir
 * date: 18/12/2023
 */

#include "value.h"
#include <stdio.h>
#include <stdlib.h>

i64_t value_to_int(value_t *v) {
    if (v->type == v_int) return ((value_int_t*)v)->val;
    else if (v->type == v_float) return (i64_t)((value_float_t*)v)->val;
    else return -1;
}

f64_t value_to_float(value_t *v) {
    if (v->type == v_int) return (f64_t)((value_int_t*)v)->val;
    else if (v->type == v_float) return ((value_float_t*)v)->val;
    else return -1;
}

char *value_to_str(value_t *v) {
    char *buf = (char*)malloc(sizeof(char) * 30);
    i32_t n = 0;
    if (v->type == v_int) {
        n = snprintf(buf, 29, "%lld", (long long)value_to_int(v)); 
    } else {
        n = snprintf(buf, 29, "%.6lf", value_to_float(v));
    }
    
    buf[n] = '\0';
    return buf;
}

void value_delete(value_t *v) {
    if (v && v->ref == 0) free(v);
}

void value_incref(value_t *v) {
    ++v->ref;
}

void value_decref(value_t *v) {
    if (v->ref == 0) return;
    --v->ref;
    if (v->ref == 0) v->delete(v);
}


value_int_t *value_int_new(i64_t val) {
    value_int_t *_n = (value_int_t*)malloc(sizeof(value_int_t));
    *_n = (value_int_t) {
        .base = {
            .type = v_int,
            .ref  = 1,
            .delete = value_delete,
        },
        .val = val,
    };

    return _n;
}

value_float_t *value_float_new(f64_t val) {
    value_float_t *_n = (value_float_t*)malloc(sizeof(value_float_t));
    *_n = (value_float_t) {
        .base = {
            .type = v_float,
            .ref  = 1,
            .delete = value_delete,
        },
        .val = val,
    };

    return _n;
}
