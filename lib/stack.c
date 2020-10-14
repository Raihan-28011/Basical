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


#include "stack.h"

extern char const *errMsg();
extern char func[];
extern void fperror();

void 
CharCreate(CharStack *this,int size)
{
    strcpy(func, "CharCreate");
    this->arr = malloc(sizeof(char)*size);

    if (this->arr) {
        this->top = -1;
        this->size = size;
    } else
        fperror(func, 6, true);
}

void 
CharPush(CharStack *this,char val)
{
    strcpy(func, "CharPush");

    if (!this->arr)
        fperror(func, 4, true);

    this->top += 1;
    this->arr[this->top] = val;
}

char 
CharTop(CharStack *this)
{
    strcpy(func, "CharTop");
    
    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }
    
    return this->arr[this->top];
}

void 
CharPop(CharStack *this)
{
    strcpy(func, "CharPop");
   
    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }
    
    this->top -= 1;
}

int 
CharSize(CharStack *this)
{
    strcpy(func, "CharSize");
    
    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }
    
    return this->top;
}

int 
CharCapacity(CharStack *this)
{
    strcpy(func, "CharCapacity");

    if (!this->arr)
        fperror(func, 4, true);

    return this->size;
}


bool 
CharEmpty(CharStack *this)
{
    strcpy(func, "CharEmpty");

    if (!this->arr)
        fperror(func, 4, true);

    return this->top == -1;
}

void 
CharDestroy(CharStack *this)
{
    strcpy(func, "CharDestroy");
    
    if (this->arr)
        free(this->arr);
    else
        fperror(func, 4, true);
}

void 
LongCreate(LongStack *this, int size)
{
    strcpy(func,"LongCreate");
    
    this->arr = malloc(sizeof(long long)*size);
    
    if (!this->arr)
        fperror(func, 6, true);
    
    this->top = -1;
    this->size = size;
}

void 
LongPush(LongStack *this,long long val)
{
    strcpy(func, "LongPush");

    if (!this->arr)
        fperror(func, 4, true);

    this->top += 1;
    this->arr[this->top] = val;
}

long long 
LongTop(LongStack *this)
{
    strcpy(func,"LongTop");
    
    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }
    
    return this->arr[this->top];
}

void 
LongPop(LongStack *this)
{
    strcpy(func,"LongPop");

    if (!this->arr)
        fperror(func, 4, true);
    
    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }

    this->top -= 1;
}

int 
LongSize(LongStack *this)
{
    strcpy(func, "LongSize");

    if (!this->arr)
        fperror(func, 4, true);
    
    return this->top;
}

int 
LongCapacity(LongStack *this)
{
    strcpy(func, "LongCapacity");

    if (!this->arr)
        fperror(func, 4, true);

    return this->size;
}

void 
LongDestroy(LongStack *this)
{
    strcpy(func,"LongDestroy");
   
    if (this->arr)
        free(this->arr);
    else
        fperror(func, 4, true);
}


void 
LDoubleCreate(LDoubleStack *this,int size)
{
    strcpy(func,"LDoubleCreate");
    
    this->arr = malloc(sizeof(long double)*size);

    if ( this->arr ) {
        this->top = -1;
        this->size = size;
    } else
        fperror(func, 6, true);
}

void 
LDoublePush(LDoubleStack *this,long double val)
{
    strcpy(func, "LDoublePush");
    
    if (!this->arr)
        fperror(func, 4, true);

    this->top += 1;
    this->arr[this->top] = val;
}

long double 
LDoubleTop(LDoubleStack *this)
{
    strcpy(func,"LDoubleTop");

    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }
    
    return this->arr[this->top];
}

void 
LDoublePop(LDoubleStack *this)
{
    strcpy(func,"LDoublePop");
    
    if (!this->arr)
        fperror(func, 4, true);

    if (this->top == -1) {
        destroy(*this);
        fperror(func, 5, true);
    }

    this->top -= 1;
}

int 
LDoubleSize(LDoubleStack *this)
{
    strcpy(func, "LDoubleSize");

    if (!this->arr)
        fperror(func, 4, true);

    return this->top;
}

int 
LDoubleCapacity(LDoubleStack *this)
{
    strcpy(func, "LDoubleCapacity");

    if (!this->arr)
        fperror(func, 4, true);

    return this->size;
}

void 
LDoubleDestroy(LDoubleStack *this)
{
    strcpy(func,"LDoubleDestroy");

    if (this->arr)
        free(this->arr);
    else
        fperror(func, 4, true);
}
