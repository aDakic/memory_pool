#include <gtest/gtest.h>

#include "memory_pool/memory_pool.h"

TEST(test_memory_pool, construction)
{
    EXPECT_NO_THROW(memory_pool::memory_pool pool(memory_pool::memory_config{sizeof(int), 2}));
    EXPECT_NO_THROW(memory_pool::memory_pool pool(memory_pool::memory_config{sizeof(int), 1000}));
    EXPECT_NO_THROW(memory_pool::memory_pool pool(memory_pool::memory_config{sizeof(long), 10}));
}

TEST(test_memory_pool, allocate_and_deallocate)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{sizeof(int), 2});

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