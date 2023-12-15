/*
 * created by: shunnobir
 * date: 30/11/2023
 */

#ifndef __BASICAL_COMMON_H__
#define __BASICAL_COMMON_H__

#define BASICAL_DEBUG 1

#include <stdint.h>

typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef float   f32_t;
typedef double  f64_t;

#ifdef __linux
#define TEXT_GREEN(x)   "\x1b[38;32m" x "\x1b[m"
#define TEXT_RED(x)     "\x1b[38;31m" x "\x1b[m"
#define TEXT_BLUE(x)    "\x1b[38;34m" x "\x1b[m"
#define TEXT_BOLD(x)    "\x1b[1m" x "\x1b[m"
#else
#define TEST_PASSED "TEST PASSED"
#define TEST_FAILED "TEST FAILED"
#define TEXT_GREEN(x)   x
#define TEXT_RED(x)     x
#endif

#endif // __BASICAL_COMMON_H__
