/*
 * created by: shunnobir
 * date: 18/12/2023
 */

#include "evaluate.h"
#include "ast.h"
#include "value.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

stack_t stack  = {
    .mem = NULL,
    .top = 0,
    .cap = 1,
};


static void stack_resize(void) {
#define STACK_GROW_FACTOR 2
    if (stack.top + 1 >= stack.cap) {
        stack.cap *= STACK_GROW_FACTOR;
        stack.mem = (value_t**)realloc(stack.mem, sizeof(value_t*) * stack.cap);
    }
}

static void stack_push(value_t *v) {
    stack_resize();
    value_incref(v);
    stack.mem[stack.top++] = v;
}

static value_t *stack_pop(void) {
    if (stack.top == 0) return NULL;
    --stack.top;
    return stack.mem[stack.top];
}

static void stack_print(void) {
#define SPACES 24
    fprintf(stderr, "|%*s%*s|\n", (SPACES-5)/2+5, "Stack", (SPACES-5+1)/2, " ");
    fprintf(stderr, "|                        |\n");
    for (i32_t i = stack.top-1; i >= 0; --i) {
        char *buf;
        fprintf(stderr, "|%*s|\n", SPACES, (buf = value_to_str(stack.mem[i])));
        if (buf) free(buf);
    }
    fprintf(stderr, "|________________________|\n\n");
}

void stack_delete(void) {
    if (stack.mem) {
        for (i32_t i = stack.top-1; i >= 0; --i) {
            value_decref(stack.mem[i]);
        }
        free(stack.mem);
    }
}

void evaluate_add(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    bool do_float = (v->type == v_float || v2->type == v_float);
    value_t *result = (value_t*)value_int_new(
        value_to_int(v)
        +
        value_to_int(v2)
    );
    if (do_float) {
        value_decref(result); /* delete the int value if allocated */
        result = (value_t*)value_float_new(
                value_to_float(v) 
                + 
                value_to_float(v2)
        );
    }
    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();
}

void evaluate_sub(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    bool do_float = (v->type == v_float || v2->type == v_float);
    value_t *result = (value_t*)value_int_new(
        value_to_int(v)
        -
        value_to_int(v2)
    );
    if (do_float) {
        value_decref(result); /* delete the int value if allocated */
        result = (value_t*)value_float_new(
                value_to_float(v) 
                - 
                value_to_float(v2)
        );
    }
    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();
}

void evaluate_mul(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    bool do_float = (v->type == v_float || v2->type == v_float);
    value_t *result = (value_t*)value_int_new(
        value_to_int(v)
        *
        value_to_int(v2)
    );
    if (do_float) {
        value_decref(result); /* delete the int value if allocated */
        result = (value_t*)value_float_new(
                value_to_float(v) 
                * 
                value_to_float(v2)
        );
    }
    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();
}

void evaluate_div(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    value_t *result = (value_t*)value_float_new(
        value_to_float(v)
        /
        value_to_float(v2)
    );

    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();

}

void evaluate_mod(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    bool do_float = (v->type == v_float || v2->type == v_float);
    value_t *result = (value_t*)value_int_new(
        value_to_int(v)
        %
        value_to_int(v2)
    );
    if (do_float) {
        value_decref(result); /* delete the int value if allocated */
        result = (value_t*)value_float_new(
                fmod(
                    value_to_float(v)
                    ,
                    value_to_float(v2)
                )
        );
    }
    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();

}

void evaluate_pow(void) {
    value_t *v2 = stack_pop();
    value_t *v  = stack_pop();

    bool do_float = (v->type == v_float || v2->type == v_float);
    value_t *result = (value_t*)value_int_new(
        pow(
            value_to_int(v)
            ,
            value_to_int(v2)
        )
    );
    if (do_float) {
        value_decref(result); /* delete the int value if allocated */
        result = (value_t*)value_float_new(
                pow(
                    value_to_float(v)
                    ,
                    value_to_float(v2)
                )
        );
    }
    stack_push(result);
    value_decref(v);
    value_decref(v2);
    value_decref(result);
    stack_print();
}

void evaluate_int(i64_t i) {
    value_t *v = (value_t*)value_int_new(i); 
    stack_push(v);
    value_decref(v);
    stack_print();
}

void evaluate_float(f64_t f) {
    value_t *v = (value_t*)value_float_new(f);
    stack_push(v);
    value_decref(v);
    stack_print();
}

void evaluate_list(ast_list_t *list) {
    value_list_t *l = value_list_new();
    for (i32_t i = 0; i < list->len; ++i) {
        list->elems[i]->evaluate(list->elems[i]);
        value_t *v = stack_pop();
        value_list_push(l, v);
        value_decref(v);
    }
    stack_push((value_t*)l);
    value_decref((value_t*)l);
    stack_print();
}
