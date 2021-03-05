//
// Created by raihan on 2/22/21.
//

#ifndef BASICAL_ERROR_H
#define BASICAL_ERROR_H

#include <stdbool.h>
#include <stddef.h>

#define NULL_CHAR (char*)0

enum { MAX_ERRORS = 10 };
extern bool error_occurred;
extern char *errors[MAX_ERRORS];
extern char *error_ind;
extern ptrdiff_t Index;

void add_error(char const *message);
void add_error_token(char const *format, ...);
void clear_errors();

#endif //BASICAL_ERROR_H
