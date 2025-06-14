CC = gcc
BINARY = bin/bank
CFLAGS = -Wall -Wextra -std=c99 -g -lcrypt

SRCS = src/main.c src/auth.c src/utils.c src/store.c src/user_store.c src/id_tracker_store.c
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

fmt:
	@clang-format -i src/*.c src/*.h
	@echo ".C and .H files formatted"
