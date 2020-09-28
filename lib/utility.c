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
/* #include "error.h" */

extern char charSet[];
extern char const *errMsg();
extern char func[];
extern void fperror();

/*static*/ int
greaterPrecedence(char char1, char char2)
{
    if ((char1 == '*' || char1 == '/' || char1 == '%') && (char2 != '*' && char2 != '/' && char2 != '%'))
        return 1;
    else if (((char1 == '*' || char1 == '/' || char1 == '%') && (char2 == '*' || char2 == '/' || char2 == '%')) ||\
             ((char1 == '+' || char1 == '-' ) && (char2 == '+' || char2 == '-')))
        return 2;
    else
        return 0;
}

/*static*/ bool
validChar(char inputChar)
{
    for (int i=0; i<5; ++i)
        if (inputChar == charSet[i])
            return true;

    return false;
}


void
convert_to_postfix (char *buf)
{
    strcpy(func, "convert_to_postfix");

    CharStack operators;
    create(operators, 10);
    
    char controlChar = ' ';
    long long number = 0;
    int charCount = 0;

    while ((controlChar = getchar()) != '\n') {
        if (isspace(controlChar))
           continue;
        else if (isdigit(controlChar)) {
            ungetc(controlChar, stdin);
            scanf("%lld", &number);
            charCount += sprintf(buf+charCount, "%lld ", number);
        } else {
           if (validChar(controlChar)) {
                if (!isempty(operators)) {
                    char check = top(operators);
                    int val = greaterPrecedence(check,controlChar);

                    if (val == 1) {
                        while (!isempty(operators)) {
                            check = top(operators);
                            charCount += sprintf(buf+charCount, "%c ", check);
                            pop(operators);
                        }
                        push(operators, controlChar);
                    } else if (val == 2) {
                        charCount += sprintf(buf+charCount, "%c ", check);
                        pop(operators);
                        push(operators, controlChar);
                    } else
                        push(operators, controlChar);
                } else
                    push(operators, controlChar);
           } else {
                destroy(operators);
                fperror(func, 1, true);
           }
        }
    }

    while (!isempty(operators)) {
        controlChar = top(operators);
        charCount += sprintf(buf+charCount, "%c ", controlChar);
        pop(operators);
    }

    destroy(operators);
}

long long
getNum(char const *buf, int pos)
{
    long long number = 0;
    sscanf(buf+pos, "%lld", &number);
    
    return number;
}

int
digits(long long number)
{
    strcpy(func, "digits");
    
    if (!number)
        fperror(func, 3, true);
    
    int counter = 0;

    while (number) {
        number /= 10;
        ++counter;
    }

    return counter;
}


inline long long
addition(long long arg1, long long arg2)
{
    return arg1 + arg2;
}

inline long long
substract(long long arg1, long long arg2)
{
    return arg1 - arg2;
}

inline long long
multiply(long long arg1, long long arg2)
{
    return arg1 * arg2;
}

inline long long
division(long long arg1, long long arg2)
{
    return arg1 / arg2;
}

inline long long
reminder(long long arg1, long long arg2)
{
    return arg1 % arg2;
}

void
calculate(LongStack *stack, char operator)
{
    long long arg2 = LongTop(stack);
    pop(*stack);

    long long arg1 = LongTop(stack);
    pop(*stack);

    long long result = 0;

    switch (operator) {
    case '+':
        result = addition(arg1, arg2);
        break;
    case '-':
        result = substract(arg1, arg2);
        break;
    case '*':
        result = multiply(arg1, arg2);
        break;
    case '/':
        result = division(arg1, arg2);
        break;
    case '%':
        result = reminder(arg1, arg2);
        break;
    default:
        ; // null statement
    }

    push(*stack, result);
}


void
evaluateExpression(LongStack *stack, char const *buf)
{
    char controlChar = ' ';
    long long number = 0;
    int position = 0;

    while (buf[position]) {
        sscanf(buf+position, "%c", &controlChar);

        if (isspace(controlChar)) {
            ++position;
            continue;
        } else if (isdigit(controlChar)) {
            sscanf(buf+position, "%lld", &number);
            
            position += digits(number);
            push(*stack, number);
        } else {
            calculate(stack, controlChar);
            ++position;
        }
    }
}
