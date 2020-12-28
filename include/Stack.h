#ifndef STACKIMP_H
#define STACKIMP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef union Data {
    char data_c;
    long long data_l;
    long double data_d;
} Data;

typedef enum tag { CHAR, LONG, FLOAT } Tag;

typedef struct Stack {
    Data data;
    struct Stack *next;
} Stack;


typedef struct numDigits {
    int digitCount;
    struct numDigits *next;
} numDigits;



/* stack interface */
Stack *stack_create(void);
void stack_push(Stack **, Data const);
Stack stack_pop(Stack **);
Stack stack_top(Stack *);
bool stack_empty(Stack *);
void stack_destruct(Stack **);



/* numDigits interface */
numDigits *numDigits_create(void);
void numDigits_push(numDigits **, int const);
int numDigits_pop(numDigits **);
void numDigits_reverse(numDigits **);
void numDigits_destruct(numDigits **);


#endif


