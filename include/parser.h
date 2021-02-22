//
// Created by raihan on 2/22/21.
//

#ifndef BASICAL_PARSER_H
#define BASICAL_PARSER_H

#include "scanner.h"
#include "expression.h"

typedef struct Parser {
  Scanner scanner;
} Parser;


ExprTree *parse(Parser *parser, char const *source);


#endif //BASICAL_PARSER_H
