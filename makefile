CC=gcc
OPT=-v -Wall -g


test: test.c utilities.o stack.o test.o
	$(CC) $(OPT) -o test utilities.o stack.o test.c

stack: utilities.h stack.h stack.c
	$(CC) $(OPT) -o stack.o -c stack.c

utilities: utilities.h utilities.c
	$(CC) $(OPT) -o utilities.o -c utilities.c

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./test

clean:
	rm *.o
