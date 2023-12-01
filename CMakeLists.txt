CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(basical C)

include_directories(include)
add_compile_options(-Wall -Wextra -pedantic -Wswitch-enum -lm -Wunused-command-line-argument)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_executable(basical src/main.c src/main.h
                       src/common.h src/error.c 
                       src/error.h src/lexer.c 
                       src/lexer.h src/token.h
                       src/token.c)
