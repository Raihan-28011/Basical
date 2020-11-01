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


#include "../lib/utility.h"
#include <stdio.h>
#include <errno.h>

int main(){
    FILE *fd = fopen(fn, md);
    fd1 = fd;
    system("clear");

    int mode = 0;
    printf("\t\t\tWelcome to Basical\n");
    printf("\t\t  github.com/Raihan-28011/Basical\n");
    printf("\t\t\tnoob.04@outlook.com\n");
    printf("\t==================================================\n");
    printf("\t1. Basic mode\t2. Binary mode\n\t3. Help\n");

    while (printf("\n\tChoose a mode: ") && scanf("%d", &mode) && (mode != 1 && mode != 2 && mode != 3))
        printf("\n\tInvalid number.\n\tTry again.\n");

    // eating the new line entered by the user, otherwise our main code will take it as input
    getchar();

    if (mode == 1) {
        char option = 'y';
        while ( option == 'y' || option == 'Y' ){
            printf("\n\t-> ");
            char buf[100];

            // this will convert input into a postfix form using stack.
            convert_to_postfix(buf);
            /* printf("%s\n",buf); */

            printf("\t--------------------------------------------------\n");

            // stack for storing the result value
            Stack stack;
            stack_create(stack, 5, LLNG);

            // this will evaluate the expression we stored into buf
            evaluateExpression(&stack, buf);

            // printing the result
            printf("\t%-48lld\n", stack_top(stack, LLNG));

            printf("\t--------------------------------------------------\n");

            // freeing memory
            stack_destroy(stack);

            // if user wants to keep on using the calculator or not
            printf("\tcontinue? [Y/n] ");
            scanf("%c",&option);
            getchar(); // this will eat the newline entered by user, otherwise our main code will take it as input
        } 
        /* system("clear"); */
    } else if (mode == 2) {
        printf("\tWork is going on\n");
    } else {
        printf("Work is going on\n");
    }
    fclose(fd);
}
