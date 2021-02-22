//
// Created by raihan on 2/22/21.
//

#include "include/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

bool error_occurred = false;
char *errors[MAX_ERRORS] = { (char*)0, (char*)0, (char*)0, (char*)0, (char*)0,
                             (char*)0, (char*)0, (char*)0, (char*)0, (char*)0 };
ptrdiff_t Index = 0;
char *error_ind = (char*)0;

void add_error(char const *message) {
  if (!error_occurred)
    error_occurred = true;

  size_t len = strlen(message);
  error_ind = (char *)malloc(sizeof(char) * (len + 1));
  strncpy(error_ind, message, len);
  errors[Index] = error_ind;
  ++Index;
}

void add_error_token(char const *format, ...) {
  if (!error_occurred)
    error_occurred = true;

  size_t len = strlen(format);
  error_ind = (char*)malloc(sizeof(char) * (len + 30)); /* 30 extra memory to store error token */

  va_list list;
  va_start(list, format);
  vsprintf(error_ind, format, list);
  va_end(list);

  errors[Index] = error_ind;
  ++Index;
}

void clear_errors() {
  ptrdiff_t ind = 0;
  while (ind != Index) {
    free(errors[ind]);
    ++ind;
  }
}
