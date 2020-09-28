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
