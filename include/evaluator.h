//
// Created by raihan on 2/22/21.
//

#ifndef BASICAL_EVALUATOR_H
#define BASICAL_EVALUATOR_H

#include "expression.h"
#include "scanner.h"

double evaluate_expression_d(Expression *root);
long long evaluate_expression_l(Expression *root);

#endif //BASICAL_EVALUATOR_H
