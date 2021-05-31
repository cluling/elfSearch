CC=gcc
OPT=-v -Wall -g

utilities: utilities.h utilities.c
	$(CC) $(OPT) -o utilities.o -c utilities.c

clean:
	rm *.o
