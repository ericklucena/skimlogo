CC=gcc
CPPFLAGS=
CFLAGS= -std=c99 -Wall -pedantic -Iinclude -lm -g
LDFLAGS= -lncurses

all: src/*.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o bin/runMe

clean: 
	rm bin/runMe
	rm bin/test
