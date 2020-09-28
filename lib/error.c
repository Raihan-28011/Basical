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


#include "error.h"

void
fperror(char const *fname,int errnum, bool ext)
{
    fprintf(stderr, "%s: %s\n", fname, errMsg(errnum)); 

    if (ext)
        exit(-1);
}

char const *
errMsg(int errnum)
{
    localErrno = errnum;
    
    switch (localErrno){
    case SUCCESS:
        strcpy(errorMassage,"No error occured");
        break;
    case EINVI:
        strcpy(errorMassage, "Invalid input character set");
        break;
    case EOUTRANGE:
        strcpy(errorMassage, "Input out of range (possible overflow)");
        break;
    case ENOPOSITIVE:
        strcpy(errorMassage, "Input is not a positive number");
        break;
    case ESTACKNOCRT:
        strcpy(errorMassage, "Stack has not been created yet");
        break;
    case ESTACKEMPT:
        strcpy(errorMassage, "Stack is empty");
        break;
    case EUCRTSTACK:
        strcpy(errorMassage, "Unable to create stack (malloc failed)");
    }

    return errorMassage;
}
