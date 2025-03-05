CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lcurl -lcjson

SRC = graph.c api.c json_parser.c
OBJ = $(SRC:.c=.o)
TARGET = graph_cli

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
