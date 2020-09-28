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
