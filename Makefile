CC = gcc
BINARY = bin/bank
CFLAGS = -Wall -Wextra -std=c99 -g -lcrypt

SRCS := $(shell find src -name "*.c")
HEADERS := $(shell find src -name "*.h")
OBJS := $(patsubst src/%.c, bin/%.o, $(SRCS))

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin

fmt:
	clang-format -i $(SRCS) $(HEADERS)
	@echo "Formatted .c and .h files"
