CC = gcc
CCFLAGS = -Wextra -ansi -pedantic -Wall -g -O3 -std=gnu11
LIBNAME = libstore.a

all : $(LIBNAME)

clean :
	rm -f *.o $(LIBNAME)

$(LIBNAME): hash.o bintree.o
	$(AR) $(ARFLAGS) $@ $^

hash.o : hash.c
	$(CC) $(CCFLAGS) -c $^

bintree.o : bintree.c
	$(CC) $(CCFLAGS) -c $^

