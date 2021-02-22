#include <stdio.h>
#include "include/error.h"
#include "include/parser.h"
#include "include/evaluator.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define NORMAL "\x1b[0m"

static bool print_errors() {
  if (error_occurred) {
    ptrdiff_t ind = 0;
    while (ind != Index)
      fprintf(stderr, RED "error: " NORMAL "%s\n", errors[ind++]);
    return true;
  }
  return false;
}

static void free_everything(ExprTree *tree) {
  delete_expression_tree(tree);
  clear_errors();
}

static void prompt() {
  printf(BLUE "⮩  " NORMAL);
}

static void print_result_d(double result) {
  char res[100];
  sprintf(res, "%lf", result);
  int len = strlen(res);

  /* remove trailing zero's */
  while (res[len - 2] != '.' && res[len-1] == '0') {
    res[len - 1] = '\0';
    --len;
  }
  printf(GREEN "⮩  " NORMAL "%s\n", res);
}

static void print_result_l(long long result) {
  printf(GREEN "⮩  " NORMAL "%lld\n", result);
}

static void hint() {
  fprintf(stdout, "To use basical, provide an expression and hit enter.\nExample:\n"
                  BLUE "  ⮩  " NORMAL "(10 ** 2) + 100[ENTER]\n"
                  GREEN "  ⮩  " NORMAL "10000\n"
                  BLUE "  ⮩  " NORMAL "10.0 + 5.5 * 5.5[ENTER]\n"
                  GREEN "  ⮩  " NORMAL "40.35\n");
}

static void help() {
  fprintf(stdout, "Type '" GREEN ".hint" NORMAL "' to see some examples\n");
}

static bool handle_commands(char const *expression) {
  if (expression[0] == '.') {
    if (strncmp(expression, ".hint", 5) == 0) {
      hint();
    } else if (strncmp(expression, ".help", 5) == 0) {
      help();
    } else {
      fprintf(stderr, RED "error" NORMAL ": unrecognised command '" GREEN "%s" NORMAL "'\n", expression);
    }
    return true;
  }
  return false;
}


enum { MAX_EXPRESSION_LEN = 1024 };

int main(void) {
  puts("Type '" GREEN ".hint" NORMAL "' or '" GREEN ".help" NORMAL "' to get started");
  
  while (true) {
    prompt();
    char expression[MAX_EXPRESSION_LEN + 1] = "";
    
    if (!fgets(expression, MAX_EXPRESSION_LEN, stdin)) {
      print_errors();
      clear_errors();
      puts("(eof)");
      return EXIT_SUCCESS;
    }
    
    if (expression[0] == '\n') {
      puts("(null expression)");
      continue;
    }
    
    expression[strlen(expression) - 1] = '\0';
    if (handle_commands(expression))
      continue;

    Parser parser;
    ExprTree *tree = parse(&parser, expression);

    if (print_errors()) {
      free_everything(tree);
      continue;
    }

    if (floatPresent) {
      double result = evaluate_expression_d(tree->_root);
      print_result_d(result);
    } else {
      long long result = evaluate_expression_l(tree->_root);
      print_result_l(result);
    }
    putchar('\n');
    
    if (print_errors()) {
      free_everything(tree);
      continue;
    }

    free_everything(tree);
  }
  return EXIT_SUCCESS;
}
