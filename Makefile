CC = gcc
BINARY = bin/bank
CFLAGS = -Iinclude -Wall -Wextra -std=c99 -g

SRCS = src/main.c
OBJS_NAMES = $(notdir $(SRCS:.c=.o)) 
BIN_OBJS = $(addprefix bin/, $(OBJS_NAMES))

all: $(BINARY)

$(BINARY): $(BIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) bin/*.o
