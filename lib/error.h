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

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// local perror
void fperror(FILE* fd, char const *fname, int errnum, bool ext, char const *file, int line);

// valid character set
char charSet[5] = { '+', '-', '/', '*', '%' };

// it works exactly like errno and works with fperror()
int localErrno = 0;

// it will store the type of error that has occured
char errorMassage[100];

// this string will store the function name in which the error has occured
char func[50] = { "Global" };

// this enum defines different local error types
enum errorType { SUCCESS,           // no error occured
                 EINVI,             // invalid input character set
                 EOUTRANGE,         // input out of range (possible overflow)
                 ENOPOSITIVE,       // input is not positive
                 ESTACKNOCRT,       // stack has not been created yet
                 ESTACKEMPT,        // stack is empty
                 EUCRTSTACK         // unable to create stack (malloc failed)
};

// this function will set the errorMassage[] string to appropriate error massage
char const *errMsg(int errnum);

#endif
