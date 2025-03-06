CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lcurl -lcjson

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = graph_cli

TEST_SRC = tests/test_graph.c
TEST_OBJ = $(BUILD_DIR)/test_graph.o
TEST_TARGET = test_graph

.PHONY: all clean test dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJ_DIR) output

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: dirs $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/test_graph.o: tests/test_graph.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

