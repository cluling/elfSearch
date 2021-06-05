CC=gcc
OPT=-v -Wall -g


test: test.c utilities.o elf.o stack.o
	$(CC) $(OPT) -o test utilities.o elf.o stack.o test.c

elfSearch: main.c utilities.o elf.o stack.o
	$(CC) $(OPT) -o elfSearch utilities.o elf.o stack.o main.c

elf: utilities.h elf.h elf.c
	$(CC) $(OPT) -o elf.o -c elf.c

stack: utilities.h stack.h stack.c
	$(CC) $(OPT) -o stack.o -c stack.c

utilities: utilities.h utilities.c
	$(CC) $(OPT) -o utilities.o -c utilities.c

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all ./elfSearch

clean:
	rm *.o; rm *.o 2>/dev/null; rm elfSearch 2>/dev/null
