# MIT License

# Copyright (c) 2020 Raihan Kabir

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


CC=clang-10
CFLAGS=-std=c2x -Wall -Wextra -pedantic

STK_SRC=include/Stack.c
UTL_SRC=include/utility.c

STK_OBJ=obj/Stack.o
UTL_OBJ=obj/utility.o

TEMP_OBJ1=obj/tempObj1.o
TEMP_OBJ2=obj/tempObj2.o

BASCL_OBJ=obj/basical.o

basical: $(UTL_OBJ) $(STK_OBJ)
	$(CC) $(CFLAGS) -c src/basical.c -o $(BASCL_OBJ)
	$(CC) $(CFLAGS) $(STK_OBJ) $(UTL_OBJ) $(BASCL_OBJ) -o bin/basical

$(STK_OBJ): $(STK_SRC)
	$(CC) $(CFLAGS) -c $(STK_SRC) -o $(STK_OBJ)

$(UTL_OBJ): $(UTL_SRC)
	$(CC) $(CFLAGS) -c $(UTL_SRC) -o $(UTL_OBJ)

run:
	bin/basical

clean:
	rm bin/basical obj/*
