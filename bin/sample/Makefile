LIBNAME = libstore.a
USER_DIR = ../../source/sample
LIB_DIR = ../../libstore
CCFLAGS = -Wextra -ansi -pedantic -Wall -g -O3 -std=gnu11 -I $(LIB_DIR)
TARGET = sample1 sample2


all : $(TARGET)

clean :
	rm -f *.o $(TARGET)

$(LIB_DIR)/$(LIBNAME) : $(LIB_DIR)/Makefile
	@make -C $(LIB_DIR)

sample1 : $(USER_DIR)/sample1.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^

sample2 : $(USER_DIR)/sample2.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^


