CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(basical C)

include_directories(include)
add_compile_options(-Wall -Wextra -pedantic -Wswitch-enum)
add_link_options(-lm)

add_executable(basical src/main.c src/main.h
                       src/common.h src/error.c 
                       src/error.h src/lexer.c 
                       src/lexer.h src/token.h
                       src/token.c)
