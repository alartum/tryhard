LIBNAME = libstore.a
USER_DIR = ../../source/sample
LIB_DIR = ../../libstore
CCFLAGS = -Wextra -ansi -Wall -O3 -std=gnu11 -I $(LIB_DIR)
TARGET_TREE = sample1_tree sample2_tree
TARGET_HASH = sample1_hash sample2_hash
MODE = none

all : tree hash

tree : $(TARGET_TREE)

hash : $(TARGET_HASH)

clean :
	rm -f *.o $(TARGET_TREE) $(TARGET_HASH)

$(LIB_DIR)/$(LIBNAME) : $(LIB_DIR)/Makefile
	@make -C $(LIB_DIR)

sample1_tree : $(USER_DIR)/sample1.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^ -DTREE_MODE

sample2_tree : $(USER_DIR)/sample2.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^ -DTREE_MODE

sample1_hash : $(USER_DIR)/sample1.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^ -DHASH_MODE

sample2_hash : $(USER_DIR)/sample2.c $(LIB_DIR)/$(LIBNAME) $(LIB_DIR)/storage.h
	$(CC) $(CCFLAGS) -o $@ $^ -DHASH_MODE

