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
