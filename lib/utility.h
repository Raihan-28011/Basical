// License
/*
 * MIT License
 *
 * Copyright (c) 2020 Raihan Kabir
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


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
