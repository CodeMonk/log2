
CFLAGS=-Wall -O2
CC=gcc

test_log2: test_log2.c log2.h
	$(CC) $(CFLAGS) -o $@ $< -lm

asm: test_log2.c log2.h
	$(CC) $(CFLAGS) -static -save-temps -o test_log2 $< -lm

clean:
	rm -f test_log2
	rm -f test_log2.exe
	rm -f test_log2.i
	rm -f test_log2.o
	rm -f test_log2.s
