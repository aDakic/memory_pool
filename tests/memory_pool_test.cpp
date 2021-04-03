#include <gtest/gtest.h>
#include "memory_pool/memory_pool.h"

TEST(test_memory_pool, construction)
{
    memory_pool pool(memory_config{4_B, 2});

    int* ptr1 = reinterpret_cast<int*>(pool.allocate(4));
    EXPECT_NE(ptr1, nullptr);
    
    *ptr1 = 42;
    EXPECT_EQ(*ptr1, 42);
    
    int* ptr2 = reinterpret_cast<int*>(pool.allocate(4));
    EXPECT_NE(ptr1, nullptr);
    
    *ptr2 = 43;
    EXPECT_EQ(*ptr2, 43);

    int* ptr3 = reinterpret_cast<int*>(pool.allocate(4));
    EXPECT_EQ(ptr3, nullptr);

    pool.deallocate(reinterpret_cast<uint8_t*>(ptr1), 4);
    ptr3 = reinterpret_cast<int*>(pool.allocate(4));
    EXPECT_NE(ptr3, nullptr);

    pool.deallocate(reinterpret_cast<uint8_t*>(ptr2), 4);
    pool.deallocate(reinterpret_cast<uint8_t*>(ptr3), 4);
}