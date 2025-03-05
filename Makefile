CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lcurl -ljansson

SRCS = main.c graph.c llm_client.c
OBJS = $(SRCS:.c=.o)
TARGET = graph_generator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
