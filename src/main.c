/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#include "main.h"
#include "error.h"
#include "lexer.h"
#include <stdlib.h>

void usage() {
    fprintf(stderr, "usage: basical [FILENAME]\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) usage();

    lexer_t *lexer = lexer_new();
    lexer_tokenize_file(lexer, argv[1]);
    lexer_print_tokens(lexer);
    lexer_delete(lexer);
}
