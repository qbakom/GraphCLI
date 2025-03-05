CC = gcc
CFLAGS = -Wall -Wextra -I./src
SRC = src/main.c src/graph.c src/ui.c src/parser.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = graph_generator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: 
	$(CC) -o test_runner tests/test_graph.c tests/test_parser.c tests/test_main.c src/graph.c src/parser.c src/utils.c
	./test_runner
	rm -f test_runner

.PHONY: all clean test