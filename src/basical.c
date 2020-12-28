#include "include/utility.h"
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc > 1) {
        if (!strcmp(argv[1], "--help") ||
            !strcmp(argv[1], "-h")) {
                printf("usage: %s [OPTION]\n"
                        "options:\n"
                        "  -h --help     shows this message\n\n"
                        "Example:\n"
                        "  $ bin/basical\n"
                        "  > 10 * 20.5 * 30.5\n"
                        "  6252.5000000000\n"
                        "  > 3232 / 2323 * 3872 + 323 * 872\n"
                        "  287043.1304347826\n"
                        "  > \n"
                        "Pressing CTRL+D (in windows CTRL+J) to exit the program\n",
                        argv[0]);
        } else {
            fprintf(stderr, "wrong arguments\n"
                            "usage: %s [OPTION]\n"
                            "options:\n"
                            "  -h --help     shows this message\n", argv[0]);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    char buf[1024];
    while (true) {
        printf("> ");
        fflush(stdout);
        char c = getchar();
        if (c == EOF)
            break;
        else if (c == '\n')
            continue;
        else
            ungetc(c, stdin);
        convert_to_postfix(buf);
        evaluateExpression(buf);
    }
    return EXIT_SUCCESS;
}
