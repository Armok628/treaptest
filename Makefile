CC=clang
CFLAGS=-g -pedantic

main:
	$(CC) $(CFLAGS) test.c src/*.c
