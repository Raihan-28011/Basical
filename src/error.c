/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char *fmt, ...) {
    fprintf(stderr, "[ERROR] ");
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    fprintf(stderr, "\n");
    exit(1);
}
