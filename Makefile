CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -Iinclude -I/usr/include/libxml2
LDFLAGS = -lxml2 -I/usr/include/libxml2

TARGET = target/bin/app
TEST_TARGET = target/bin/test_runner
OBJ_DIR = target/obj
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(OBJ_DIR)/test_runner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p target/bin
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/test_runner.o: tests/test_runner.c
	@mkdir -p target/obj
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	@echo "GRUZITSYAA...."
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p target/bin
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) -h

clean:
	rm -rf target/obj/* target/bin/*

.PHONY: all test valgrind clean

docs:
	doxygen Doxyfile
