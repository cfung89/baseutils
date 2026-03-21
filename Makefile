CC = gcc
FLAGS = -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.c)
OBJ_DIR = build
OBJ = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: clean $(OBJ) $(TEST_BIN)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf build

.PHONY: all test clean
