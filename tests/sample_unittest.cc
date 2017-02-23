#include "gtest/gtest.h"
#include "storage.h"

TEST(SizeTest, Empty) {
    storage_t* temp = init_hash(sizeof(size_t), 1024, 64, NULL, NULL);

    EXPECT_EQ(temp->size(temp), 0);
}

TEST(SizeTest, NonEmpty) {
    storage_t* temp = init_hash(sizeof(size_t), 1024, 64, NULL, NULL);
    char str[] = "Test";
    int x = 0;
    temp->add(temp, str, &x);
    
    EXPECT_EQ(temp->size(temp), 1);
}
