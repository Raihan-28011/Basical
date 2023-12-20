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
    char *buf;
    i32_t n = 0;
    switch (v->type) {
    case v_int:
        buf = (char*)malloc(sizeof(char) * 30);
        n = snprintf(buf, 29, "%lld", (long long)value_to_int(v)); 
        break;
    case v_float:
        buf = (char*)malloc(sizeof(char) * 30);
        n = snprintf(buf, 29, "%.6lf", value_to_float(v));
        break;
    case v_list:
    {
        buf = (char*)malloc(sizeof(char) * 1025);
        n = snprintf(buf, 1025, "[");
        value_list_t *l = (value_list_t*)v;
        for (i32_t i = 0; i < l->len; ++i) {
            char *tbuf = value_to_str(l->elems[i]);
            n += snprintf(buf + n, 1025, " %s", tbuf);
            if (i < l->len-1) n += snprintf(buf + n, 1025, ",");
            free(tbuf);
        }
        n += snprintf(buf + n, 1025, " ]"); 
        break;
    }
    default:
        break;
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

static void value_list_delete(value_t *v) {
    value_list_t *l = (value_list_t*)v;
    for (i32_t i = 0; i < l->len; ++i) {
        value_decref(l->elems[i]);
    }
    free(l->elems);
    free(l);
}

static void value_list_resize(value_list_t *list) {
#define VALUE_LIST_GROW_FACTOR 2
    if (list->len + 1 >= list->cap) {
        list->cap  *= VALUE_LIST_GROW_FACTOR;
        list->elems = (value_t**)realloc(list->elems, sizeof(value_t*) * list->cap);
    }
}

value_list_t *value_list_push(value_list_t *list, value_t *v) {
    value_list_resize(list);
    value_incref(v);
    list->elems[list->len++] = v;
    return list;
}

value_list_t *value_list_new(void) {
    value_list_t *_n = (value_list_t*)malloc(sizeof(value_list_t));
    *_n = (value_list_t) {
        .base = {
            .type = v_list,
            .ref  = 1,
            .delete = value_list_delete,
        },
        .elems = NULL,
        .len   = 0,
        .cap   = 1,
    };
    return _n;
}
