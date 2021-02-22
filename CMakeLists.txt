CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(basical C)

include_directories(include)
add_compile_options(-Wall -Wextra)
add_link_options(-lm)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER clang-11)
set(CMAKE_C_STANDARD 11)

add_executable(basical main.c include/token.h include/scanner.h scanner.c
                include/expression.h expression.c include/error.h error.c
                include/parser.h parser.c include/evaluator.h evaluator.c)
