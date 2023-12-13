/*
 * created by: shunnobir
 * date: 02/12/2023
 */

#ifndef __BASICAL_TEST_H__
#define __BASICAL_TEST_H__

#include "../src/common.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef __linux
#define TEST_PASSED     "\x1b[38;32m PASSED \x1b[m"
#define TEST_FAILED     "\x1b[38;31m FAILED \x1b[m"
#define TEXT_GREEN(x)   "\x1b[38;32m" x "\x1b[m"
#define TEXT_RED(x)     "\x1b[38;31m" x "\x1b[m"
#else
#define TEST_PASSED "TEST PASSED"
#define TEST_FAILED "TEST FAILED"
#define TEXT_GREEN(x)   x
#define TEXT_RED(x)     x
#endif

typedef struct tester tester_t;

struct tester {
    i32_t tests;
    i32_t passed;
    i32_t errors;
    bool  should_exit;
};

extern tester_t g_tester;

#define TEST_INTERNAL(expr, expected, do_exit) \
    do { \
        ++g_tester.tests; \
        ++g_tester.errors; \
        if (g_tester.should_exit) { --g_tester.errors; break; }\
        if ((expr) != expected) { \
            fprintf(stderr, "(Test #%d) [Testing %s]:%s\n", \
                    g_tester.tests, #expr, TEST_FAILED); \
            if (do_exit) g_tester.should_exit = do_exit;\
        } else { \
            --g_tester.errors; \
            ++g_tester.passed; \
            fprintf(stderr, "(Test #%d) [Testing %s]:%s\n", \
                    g_tester.tests, #expr, TEST_PASSED); \
        } \
    } while(0)

#define TEST(expr, expected) \
        TEST_INTERNAL(expr, expected, 1)

#define TRY(expr, expected) \
        TEST_INTERNAL(expr, expected, 0)

#define TEST_RESULT() \
    do { \
        fprintf(stderr, "-----------------------------------\n"); \
        fprintf(stderr, "%s (%d Total, " TEXT_GREEN("%d") " Passed, " TEXT_RED("%d") " Failed, %d Skipped)\n", \
                (g_tester.errors == 0 ? TEXT_GREEN("All Tests Passed") : TEXT_RED("Tests Failed")), \
                g_tester.tests, g_tester.passed, g_tester.errors, \
                g_tester.tests - g_tester.passed - g_tester.errors \
                ); \
    } while(0)

#endif // __BASICAL_TEST_H__
