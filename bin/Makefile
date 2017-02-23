CC = gcc
CCFLAGS = -Wextra -ansi -pedantic -Wall -g -O3 -std=gnu11
TARGET = sample1 sample2
LIBNAME = libstore.a
USER_DIR = ../source

all : $(TARGET)

clean :
	rm -f *.o $(TARGET) $(LIBNAME)

$(LIBNAME): hash.o
	$(AR) $(ARFLAGS) $@ $^

sample1 : $(USER_DIR)/sample1.c $(LIBNAME)
	$(CC) $(CCFLAGS) -o $@ $^

hash.o : $(USER_DIR)/hash.c
	$(CC) $(CCFLAGS) -c $^

sample2 : $(USER_DIR)/sample2.c $(LIBNAME)
	$(CC) $(CCFLAGS) -o $@ $^


