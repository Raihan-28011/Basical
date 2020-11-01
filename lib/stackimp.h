#ifndef STACKIMP_H
#define STACKIMP_H

#include <stdlib.h>
#include <stdio.h>

typedef long long sssize_t;
// stack struct
typedef struct Stack {
    // our main array that will work as stack under the hoos
    void *mem;
    // this will indicate how many elements our stack has
    size_t size; 
    // this will indicate the index of the top element of the stack
    sssize_t top; 
} Stack;

#define fn "basical.log"
#define md "a"

// defining macros to help us make generic stack
#define INT     int
#define LNG     long
#define LLNG    long long
#define FLT     float
#define DBL     double
#define LDBL    long double

// this function like macro will allocate memory to out stack. 
// This is a fixed size stack. User has to specify the size and type of the stack
#define STACK_CRT(STACK, SIZE, TYPE)                                                                                        \
                                    do {                                                                                    \
                                        (STACK).mem = malloc(sizeof(TYPE)*SIZE);                                            \
                                        if ((STACK).mem) {                                                                  \
                                            (STACK).size = SIZE;                                                            \
                                            (STACK).top = -1;                                                               \
                                        } else {                                                                            \
                                            fprintf(fd1, "Date: %s\t\t\tTime: %s\t", __DATE__, __TIME__);                        \
                                            fprintf(fd1, "\nFile: %s\tat line: %d\tin function:stack_create\terror: malloc failed\n", __FILE__, __LINE__);       \
                                        }                                                                                   \
                                    } while(0);

// frees the allocated memory for stack
#define stack_destroy(stk) do {                                                                                                     \
                                 if ((stk).mem) {                                                                                   \
                                    free((stk).mem);                                                                                \
                                    (stk).mem = 0;                                                                                  \
                                 } else {                                                                                           \
                                     fprintf(fd1, "Date: %s\t\t\tTime: %s\t", __DATE__, __TIME__);                                       \
                                     fprintf(fd1, "\nFile: %s\tat line: %d\tin function: stack_destroy\terror: stack has not been created\n", __FILE__, __LINE__);        \
                                 }                                                                                                  \
                             } while(0);

// This function like macro pushes elements into the stack.
// User has to specify the type of the value.
// This functions doesnt check types.
// If the main stack type and push function type doesnt match,
// this function doesn't complain.
// User has to be careful about these things.
#define STACK_PUSH(STACK, VAL, TYPE)                                                                                    \
                                 do {                                                                                   \
                                     if ((STACK).top+1 > (sssize_t)(STACK).size) {                                      \
                                        fprintf(fd1, "Date: %s\t\t\tTime: %s\t", __DATE__, __TIME__);                        \
                                        fprintf(fd1, "\nFile: %s\tat line: %d\tin function: stack_push\t error: stack overflow\n", __FILE__, __LINE__);        \
                                     } else {                                                                           \
                                        ++(STACK).top;                                                                  \
                                        ((TYPE*)(STACK).mem)[(STACK).top]= VAL;                                         \
                                     }                                                                                  \
                                  } while(0);


// this function like macro returns the top element of the stack
#define STACK_TOP(STACK, TYPE) ((TYPE*)(STACK).mem)[(STACK).top]


/* these are the wrappers around main implementation of the stck interface functions*/

// allocates memory and initializes other data members
#define stack_create(stk, size, type) STACK_CRT(stk, size, type)

// pushes val to the stack
#define stack_push(stk, val, type) STACK_PUSH(stk, val, type)

// returns the top element of stack
#define stack_top(stk, type) STACK_TOP(stk, type)

// removes the top element of the stack
#define stack_pop(stk) do {                                                                                 \
                            if ((stk).top-1 < (sssize_t)-1) {                                               \
                                fprintf(fd1, "Date: %s\t\t\tTime: %s\t", __DATE__, __TIME__);                    \
                                fprintf(fd1, "\nFile: %s\tat line: %d\tin function: stack_pop\terror: stack is empty\n", __FILE__, __LINE__);     \
                             } else {                                                                       \
                                --(stk).top;                                                                \
                             }                                                                              \
                         } while(0);

// returns true if the stack is empty, false otherwise
#define stack_empty(stk) ((int)(stk).top == -1)

#endif


