#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// local perror
void fperror(char const *fname, int errnum, bool ext);

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
