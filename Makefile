
CFLAGS=-Wall
CC=gcc

test_log2: test_log2.c log2.h
	$(CC) $(CFLAGS) -o $@ $<

asm: test_log2.c log2.h
	$(CC) $(CFLAGS) -save-temps -static -o test_log2.exe $< -lm
