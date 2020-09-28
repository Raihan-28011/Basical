#include <stdio.h>
#include <errno.h>
#include "../lib/utility.h"

int main(){
    char option = 'y';
    while ( option == 'y' || option == 'Y' ){
        printf("\n-> ");
        char buf[100];
        
        // this will convert input into a postfix form using stack.
        convert_to_postfix(buf);
        /* printf("%s\n",buf); */
        
        // stack for storing the result value
        LongStack stack;
        create(stack,5);
        
        // this will evaluate the expression we stored into buf
        evaluateExpression(&stack, buf);
        
        // printing the result
        printf("\n> %lld\n",top(stack));
        
        // freeing memory
        destroy(stack);
        
        // if user wants to keep on using the calculator or not
        printf("\ncontinue? [Y/n] ");
        scanf("%c",&option);
        getchar(); // this will store the newline entered by user, otherwise our main code will take it as input
    } 
}
