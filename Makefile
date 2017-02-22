CC = gcc
CCFLAGS = -Wextra -ansi -pedantic -Wall -g -O3 -std=gnu11

all : test

Debug : tryhard

Release: tryhard

run : tryhard
	@./test

clean :
	rm -f *.o

lib_storage : hash.o
	ar r lib_storage.a hash.o 

test : lib_storage task_1_hash.c
	gcc -o test task_1_hash.c lib_storage.a

tryhard : main.o rbtree.o hash.o
	$(CC) $(CCFLAGS) -o tryhard main.o rbtree.o hash.o

main.o : main.c storage.h
	$(CC) $(CCFLAGS) -c main.c 

hash.o : hash.c
	$(CC) $(CCFLAGS) -c hash.c

rbtree.o : rbtree.c
	$(CC) $(CCFLAGS) -c rbtree.c

