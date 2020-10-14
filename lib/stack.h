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


#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

// char stack
typedef struct {
    char *arr;
    int top;
    int size;
} CharStack;

// long long stack
typedef struct {
    long long *arr;
    int top;
    int size;
} LongStack;

// long double stack
typedef struct {
    long double *arr;
    int top;
    int size;
} LDoubleStack;

// char stack
void CharCreate(CharStack *This,int size);

bool CharEmpty(CharStack *This);

void CharPush(CharStack *This, char val);

void CharPop(CharStack *This);

char CharTop(CharStack *This);

int CharSize(CharStack *This);

int CharCapacity(CharStack *This);

void CharDestroy(CharStack *This);

// long long stack
void LongCreate(LongStack *This,int size);

bool LongEmpty(LongStack *This);

void LongPush(LongStack *This, long long val);

void LongPop(LongStack *This);

long long LongTop(LongStack *This);

int LongSize(LongStack *This);

int LongCapacity(LongStack *This);

void LongDestroy(LongStack *This);

// long double stack
void LDoubleCreate(LDoubleStack *This,int size);

bool LDoubleEmpty(LDoubleStack *This);

void LDoublePush(LDoubleStack *This, long double val);

void LDoublePop(LDoubleStack *This);

long double LDoubleTop(LDoubleStack *This);

int LDoubleSize(LDoubleStack *This);

int LDoubleCapacity(LDoubleStack *This);

void LDoubleDestroy(LDoubleStack *This);


#define create(A,b) _Generic((A), CharStack: CharCreate,\
                                  CharStack*: CharCreate,\
                                  LongStack: LongCreate,\
                                  LongStack*: LongCreate,\
                                  LDoubleStack: LDoubleCreate,\
                                  LDoubleStack*: LDoubleCreate) (&(A),b)

#define push(A,b) _Generic((A), CharStack: CharPush,\
                                CharStack*: CharPush,\
                                LongStack: LongPush,\
                                LongStack*: LongPush,\
                                LDoubleStack: LDoublePush,\
                                LDoubleStack*: LDoublePush) (&(A),(b))

#define pop(A) _Generic((A), CharStack: CharPop,\
                             CharStack*: CharPop,\
                             LongStack: LongPop,\
                             LongStack*: LongPop,\
                             LDoubleStack: LDoublePop,\
                             LDoubleStack*: LDoublePop) (&(A))



#define size(A)  _Generic((A),  CharStack: CharSize,\
                                CharStack*: CharSize,\
                                LongStack: LongSize,\
                                LongStack*: LongSize,\
                                LDoubleStack: LDoubleSize,\
                                LDoubleStack*: LDoubleSize) (&(A))

#define destroy(A)  _Generic((A), CharStack: CharDestroy,\
                                  CharStack*: CharDestroy,\
                                  LongStack: LongDestroy,\
                                  LongStack*: LongDestroy,\
                                  LDoubleStack: LDoubleDestroy,\
                                  LDoubleStack*: LDoubleDestroy) (&(A))

#define top(A) _Generic((A), CharStack: CharTop,\
                             CharStack*: CharTop,\
                             LongStack: LongTop,\
                             LongStack*: LongTop,\
                             LDoubleStack: LDoubleTop,\
                             LDoubleStack*: LDoubleTop) (&(A))

#define capacity(A,b) _Generic((A), CharStack: CharCapacity,\
                                    CharStack*: CharCapacity,\
                                    LongStack: LongCapacity,\
                                    LongStack*: LongCapacity,\
                                    LDoubleStack: LDoubleCapacity,\
                                    LDoubleStack*: LDoubleCapacity) (&(A))


#define isempty(A) _Generic((A), CharStack: CharEmpty,\
                                 LongStack: LongEmpty,\
                                 LDoubleStack: LDoubleEmpty) (&(A))
#endif
