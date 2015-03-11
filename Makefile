
CFLAGS=-Wall -O
CC=gcc

test_log2: test_log2.c log2.h
	$(CC) $(CFLAGS) -o $@ $<

asm: test_log2.c log2.h
	$(CC) -static -save-temps -o test_log2 $< 

clean:
	rm -f test_log2.exe
	rm -f test_log2.i
	rm -f test_log2.o
	rm -f test_log2.s
