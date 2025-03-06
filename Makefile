CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lcurl -lcjson

SRC = main.c graph.c api.c json_parser.c
OBJ = $(SRC:.c=.o)
TARGET = graph_cli

TEST_SRC = test_graph.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_TARGET = test_graph

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) graph.o json_parser.o api.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)
