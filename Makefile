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

SRC_PATH = src
LIB_PATH = lib
BIN_PATH = bin
OBJ_PATH = obj

STACK_SRC = $(LIB_PATH)/stack.c
STACK_OBJ = $(OBJ_PATH)/stack.o
STACK_HDR= $(LIB_PATH)/stack.h

UTILITY_SRC = $(LIB_PATH)/utility.c
UTILITY_OBJ = $(OBJ_PATH)/utility.o
UTILITY_HDR = $(LIB_PATH)/utility.h

ERROR_SRC = $(LIB_PATH)/error.c
ERROR_OBJ = $(OBJ_PATH)/error.o
ERROR_HDR = $(LIB_PATH)/error.h

SRC_FILE = basical
TARGET = $(BIN_PATH)/$(SRC_FILE)

OBJS = $(STACK_OBJ) $(UTILITY_OBJ) $(ERROR_OBJ)
BINS = $(TARGET)

sample: $(STACK_OBJ) $(UTILITY_OBJ) # $(STACK_HDR) $(UTILITY_HDR) 
	# @cd /home/raihan/repos/calc/bin
	$(CC) $(CFLAGS)\
		-o $(TARGET) $(STACK_OBJ) $(UTILITY_OBJ) $(ERROR_OBJ) $(SRC_PATH)/$(SRC_FILE).c

$(STACK_OBJ): $(ERROR_OBJ)
	$(CC) $(CFLAGS) -c -o $(STACK_OBJ) $(STACK_SRC)

$(UTILITY_OBJ): $(ERROR_OBJ)
	$(CC) $(CFLAGS) -c -o $(UTILITY_OBJ) $(UTILITY_SRC)

$(ERROR_OBJ): $(ERROR_HDR)
	$(CC) $(CFLAGS) -c -o $(ERROR_OBJ) $(ERROR_SRC)

run:
	./$(TARGET)

clean:
	rm $(OBJS) $(BINS)
