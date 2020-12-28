#include "Stack.h"
#include <assert.h>
#include <stdlib.h>

Stack *stack_create(void) {
    Stack *t = (Stack*)malloc(sizeof *t);
    t->next = NULL;
    return t;
}

void stack_push(Stack **st, Data const data) {
    Stack *t = stack_create();
    t->data = data;
    t->next = (*st);
    (*st) = t;
}

Stack stack_pop(Stack **st) {
    Stack *t = (*st);
    if (t == NULL) {
        fprintf(stderr, "wrong formatted expression\n");
        exit(EXIT_FAILURE);
    }

    Stack *g = t;
    t = t->next;
    (*st) = t;
    Stack d = *g;
    free(g);
    return d;
}

Stack stack_top(Stack *st) {
    if (st == NULL) {
        fprintf(stderr, "wrong formatted expression\n");
        exit(EXIT_FAILURE);
    }
    return *st;
}

bool stack_empty(Stack *st) {
    return st == NULL;
}

void stack_destruct(Stack **st) {
    Stack *t = (*st);
    while (t != NULL) {
        Stack *g = t;
        t = t->next;
        free(g);
    }
}

numDigits *numDigits_create(void) {
    numDigits *t = (numDigits *)malloc(sizeof *t);
    t->next = NULL;
    return t;
}

void numDigits_push(numDigits **nd, int const count) {
    numDigits *t = numDigits_create();
    t->digitCount = count;
    t->next = (*nd);
    (*nd) = t;
}

int numDigits_pop(numDigits **nd) {
    numDigits *t = (*nd);
    assert(t != NULL);
    numDigits *g = t;
    (*nd) = t->next;
    int ret = g->digitCount;
    free(g);
    return ret;
}


void numDigits_reverse(numDigits **nd) {
    numDigits *t = (*nd), *d = (*nd);
    while (d->next != NULL) {
        numDigits *g = d->next;
        d->next = g->next;
        g->next = t;
        t = g;
    }
    (*nd) = t;
}

/* 

  /----\
5  4    3 -> 2 -> 1
 ^_| \
      |
      |
t > ---

*/
void numDigits_destruct(numDigits **nd) {
    numDigits *t = (*nd);
    while (t != NULL) {
        numDigits *g = t;
        t = t->next;
        free(g);
    }
}
