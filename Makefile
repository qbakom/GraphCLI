CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = -lcurl -lcjson

SOURCES = neighbor_list.c api.c json_parser.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = graph_cli

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean