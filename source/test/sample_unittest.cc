#include "gtest/gtest.h"
#include "storage.h"
#include "stdio.h"

TEST(TreeSize, Empty) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_int);

    EXPECT_EQ(bin_tree->size(bin_tree), 0);
}

TEST(TreeSize, NonEmpty) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char str[] = "Test";
    int x = 0;

    bin_tree->add(bin_tree, str, &x);
    EXPECT_EQ(bin_tree->size(bin_tree), 1);
}

TEST(TreeSize, Five) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);

    char const* strings[] = {"One", "Two", "Three", "Four", "Five"};
    int  numbers[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
        bin_tree->add(bin_tree, &strings[i], &numbers[i]);

    EXPECT_EQ(bin_tree->size(bin_tree), 5);
}

TEST(TreeAddString, Distinct) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char const* strings[] = {"One", "Two", "Three", "Four", "Five"};
    int   numbers[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
        bin_tree->add(bin_tree, &strings[i], &numbers[i]);

    for (int i = 0; i < 5 - 1; i++)
        bin_tree->drop(bin_tree, &numbers[i]);
}

TEST(TreeAddString, Repeatable) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char const* strings[] = {"One", "Two", "Three", "Four", "Five"};
    int   numbers[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
        bin_tree->add(bin_tree, &strings[i], &numbers[i]);

    for (int i = 0; i < 5; i++)
        EXPECT_EQ(bin_tree->add(bin_tree, &strings[i], &numbers[i]), 0);
}

TEST(TreeDropString, Existing) {
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char const* strings[] = {"One", "Two", "Three", "Four", "Five"};
    int   numbers[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
        bin_tree->add(bin_tree, &strings[i], &numbers[i]);

    for (int i = 0; i < 5; i++)
        bin_tree->drop(bin_tree, &strings[i]);

    for (int i = 0; i < 5; i++){
        int is_found;
        if (bin_tree->find(bin_tree, &strings[i]))
            is_found = 1;
        else
            is_found = 0;
        EXPECT_FALSE(is_found);
    }
}

TEST(TreeDropString, NotExisting){
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char const* strings[] = {"One", "Two", "Three", "Four", "Five"};
    int   numbers[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
        bin_tree->drop(bin_tree, &strings[i]);
}
