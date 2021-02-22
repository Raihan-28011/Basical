//
// Created by raihan on 2/22/21.
//

#ifndef BASICAL_SCANNER_H
#define BASICAL_SCANNER_H

#include "token.h"

typedef struct Scanner {
  char *_source;
  char *_start;
  char *_current;
  ptrdiff_t _len;
} Scanner;

bool floatPresent = false;

Token next_token(Scanner *scanner);


#endif //BASICAL_SCANNER_H
