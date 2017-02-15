CCFLAGS += -c -g -Wall -Wextra -O3


all : storage

clean :
	rm -f *.o storage

storage : main.o api.o
	$(CC) $(CCFLAGS) main.o storage.o -o storage

main.o : main.c
	$(CC) $(CCFLAGS) main.c

storage.o : hash.o rbtree.o
	$(CC) $(CCFLAGS) storage.o hash.o rbtree.o

hash.o : hash.c
	$(CC) $(CCFLAGS) hash.c

rbtree.o : rbtree.c
	$(CC) $(CCFLAGS) hash.c
