CC=clang
CFLAGS=-g

main:
	$(CC) $(CFLAGS) main.c src/*.c
