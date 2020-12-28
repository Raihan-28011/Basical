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


#include "utility.h"
#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static char charSet[5] = { '+', '-', '/', '*', '%' };
static bool doubleNums = false;  /* flag for floating point numbers */
static int precision = 0;  /* if doubleNums is true, then precision is used */
numDigits *NumDigits = NULL;

int greaterPrecedence(char char1, char char2) {
    if ((char1 == '*' || char1 == '/' || char1 == '%') &&
            (char2 != '*' && char2 != '/' && char2 != '%')) {
        return 1;
    } else if (((char1 == '*' || char1 == '/' || char1 == '%') &&
                (char2 == '*' || char2 == '/' || char2 == '%')) ||
               ((char1 == '+' || char1 == '-') &&
                (char2 == '+' || char2 == '-'))) {
        return 2;
    }

    return 0;
}

bool validChar(char inputChar) {
    for (int i = 0; i < 5; ++i)
        if (inputChar == charSet[i])
            return true;
    return false;
}

int number(char *buf, int const count) {
    int retCount = 0;
    Data n;
    char Num[20];
    char c = getchar(); 
    while (isdigit(c)) {
        Num[retCount++] = c;
        c = getchar();
    }
    
    if (c == '.') {
        doubleNums = true;
        Num[retCount++] = c;
        int precisionCount = 0;

        while (isdigit((c = getchar()))) {
            Num[retCount++] = c;
            ++precisionCount;
        }

        ungetc(c, stdin);
        Num[retCount] = '\0';
        n.data_d = strtold(Num, NULL);
        sprintf(buf+count, "%.*LF ", precisionCount, n.data_d);
        ++retCount; // incrementing for 1 extra space in buf
    } else {
        ungetc(c, stdin);
        Num[retCount] = '\0';
        n.data_l = strtoll(Num, NULL, 10);
        sprintf(buf+count, "%lld ", n.data_l);
        ++retCount;
    }
    numDigits_push(&NumDigits, retCount-1);  /* recording numbers length */
    return retCount;
}

int Operators(char *buf, Stack **st, char const c, int const count) {
    char cc = c;
    int retCount = count;
    if (!stack_empty(*st)) {
        char check = stack_top(*st).data.data_c;
        int precedence = greaterPrecedence(check, cc);

        Data const data = { .data_c = cc };
        if (precedence == 1) {
            while (!stack_empty(*st)) {
                char p = stack_pop(st).data.data_c;
                retCount += sprintf(buf+retCount, "%c ", p);
            }
            stack_push(st, data);
        } else if (precedence == 2) {
            char p = stack_pop(st).data.data_c;
            retCount += sprintf(buf+retCount, "%c ", p);
            stack_push(st, data);
        } else {
            stack_push(st, data);
        }
    } else {
        Data const data = { .data_c = cc };
        stack_push(st, data);
    }
    return retCount;
}

void convert_to_postfix(char *buf) {
    char controlChar = ' ';
    int charCount = 0;
    Stack *operators = NULL;

    while ((controlChar = getchar()) != '\n' && controlChar != EOF) {
        switch (controlChar) {
        case ' ':
        case '\t':
        case '\r':
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            ungetc(controlChar, stdin);
            charCount += number(buf, charCount);
            break;
        default:
            if (validChar(controlChar)) {
                charCount = Operators(buf, &operators, controlChar, charCount);
            }
            else {
                fprintf(stderr, "invalid character\n");
                stack_destruct(&operators);
                numDigits_destruct(&NumDigits);
                exit(-1);
            }
                break;
        }
    }
    if (stack_empty(operators)) {
        numDigits_destruct(&NumDigits);
        stack_destruct(&operators);
        fprintf(stderr, "invalid expresssion [missing operators]\n");
        exit(-1);
    }

    while (!stack_empty(operators)) {
        controlChar = stack_pop(&operators).data.data_c;
        charCount += sprintf(buf + charCount, "%c ", controlChar);
    }
}


Data addition(Data arg1, Data arg2) {
    Data ret;
    if (doubleNums) {
        ret.data_d = arg1.data_d + arg2.data_d;
    } else {
        ret.data_l = arg1.data_l + arg2.data_l;
    }
    return ret;
}

Data substract(Data arg1, Data arg2) {
    Data ret;
    if (doubleNums) {
        ret.data_d = arg1.data_d - arg2.data_d;
    } else {
        ret.data_l = arg1.data_l - arg2.data_l;
    }
    return ret;
}

Data multiply(Data arg1, Data arg2) {
    Data ret;
    if (doubleNums) {
        ret.data_d = arg1.data_d * arg2.data_d;
    } else {
        ret.data_l = arg1.data_l * arg2.data_l;
    }
    return ret;
}

Data division(Data arg1, Data arg2) {
    Data ret;
    if (doubleNums) {
        assert(arg2.data_d != 0);
        ret.data_d = arg1.data_d / arg2.data_d;
    } else {
        assert(arg2.data_l != 0);
        ret.data_l = arg1.data_l / arg2.data_l;
    }
    return ret;
}

void evaluateExpression(char const *buf) {
    Stack *st = NULL;  /* to store numbers, if encountered */
    int i = 0;  /* index counter for buf */
    numDigits_reverse(&NumDigits);  /* reversing the list to get the lengths in input order */

    while (buf[i] != '\0') {
        switch (buf[i]) {
        case ' ':
            ++i;
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                Data data;
                if (doubleNums) {
                    sscanf(buf+i, "%LF", &data.data_d);
                } else {
                    sscanf(buf+i, "%lld", &data.data_l);
                }
                stack_push(&st, data);
            }
            i += numDigits_pop(&NumDigits);  /* adding length of the number */
            break;
        case '+':
            stack_push(&st, addition(stack_pop(&st).data, stack_pop(&st).data));
            ++i;
            break;
        case '-':
            {
                Data data2 = stack_pop(&st).data;  /* order matters, 2 - 3 and 3 - 2 is different */
                Data data1 = stack_pop(&st).data;
                stack_push(&st, substract(data1, data2));
            }
            ++i;
            break;
        case '/':
            precision = 10;
            doubleNums = true;
            {
                Data data2 = stack_pop(&st).data;  /* order matters */
                Data data1 = stack_pop(&st).data;
                stack_push(&st, division(data1, data2));
                ++i;
            }
            break;
        case '*':
            if (doubleNums && !precision)
                precision = 10;  /* if there is floating point envolved than set the precision */
            stack_push(&st, multiply(stack_pop(&st).data, stack_pop(&st).data));
            ++i;
            break;
        default:
            fprintf(stderr, "wrong formatted postfix expression\n");
            stack_destruct(&st);
            numDigits_destruct(&NumDigits);
            exit(-1);
        }
    }
    if (doubleNums) {
        printf("%.*LF\n", precision, stack_pop(&st).data.data_d);
    } else {
        printf("%lld\n", stack_pop(&st).data.data_l);
    }

    /* to free the unfreed memory, if any */
    stack_destruct(&st);
    numDigits_destruct(&NumDigits);

    /* reset all globals for next iteration */
    doubleNums = false;
    precision = 0;
    NumDigits = NULL;
}
