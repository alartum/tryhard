CC = gcc
CCFLAGS = -Wextra -ansi -pedantic -Wall -g -O3 -std=gnu11

all : tryhard

Debug : tryhard

Release: tryhard

run : tryhard
	@./tryhard

clean :
	rm -f *.o

libstore : hash.o
	ar r 

tryhard : main.o rbtree.o hash.o
	$(CC) $(CCFLAGS) -o tryhard main.o rbtree.o hash.o

main.o : main.c storage.h
	$(CC) $(CCFLAGS) -c main.c 

hash.o : hash.c
	$(CC) $(CCFLAGS) -c hash.c

rbtree.o : rbtree.c
	$(CC) $(CCFLAGS) -c rbtree.c

