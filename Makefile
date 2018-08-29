CC=clang
CFLAGS=-g

main:
	$(CC) $(CFLAGS) test.c src/*.c
