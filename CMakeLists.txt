CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(basical C)

add_compile_options(-Wall -Wextra -pedantic -Wswitch-enum -gdwarf-4)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_executable(basical src/main.c src/main.h
                       src/common.h src/error.c 
                       src/error.h src/lexer.c 
                       src/lexer.h src/token.h
                       src/parser.h src/parser.c
                       src/ast.h src/ast.c
                       src/token.c)

# unit testing
add_executable(utest tests/test.h tests/test.c
                       src/common.h src/error.c 
                       src/error.h src/lexer.c 
                       src/lexer.h src/token.h
                       src/parser.h src/parser.c
                       src/ast.h src/ast.c
                       src/token.c)
