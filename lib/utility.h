#ifndef UTILITY_HDR
#define UTILITY_HDR

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

bool validChar(char inputChar);

int greaterPrecedence(char char1, char char2);

void convert_to_postfix(char *buf);

long long getNum(char const *buf, int pos);

int digits(long long number);

long long multiply(long long arg1, long long arg2);

long long addition(long long arg1, long long arg2);

long long substract(long long arg1, long long arg2);

long long division(long long arg1, long long arg2);

long long reminder(long long arg1, long long arg2);

void calculate(LongStack *stack, char opr);

void evaluateExpression(LongStack *stack, char const *buf);

#endif
