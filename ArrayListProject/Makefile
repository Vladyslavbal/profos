CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src
OBJ = obj
BIN = bin
TARGET = $(BIN)/arraylist

SOURCES = $(SRC)/main.c $(SRC)/arraylist.c
OBJECTS = $(OBJ)/main.o $(OBJ)/arraylist.o

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJ) $(BIN)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

.PHONY: all clean dirs

