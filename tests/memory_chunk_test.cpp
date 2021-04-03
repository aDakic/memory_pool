#include <gtest/gtest.h>
#include "memory_pool/memory_chunk.h"

TEST(test_memory_chunk, default_noexept_construction)
{
    EXPECT_NO_THROW(memory_chunk chunk());
}

TEST(test_memory_chunk, construction)
{
    EXPECT_NO_THROW(memory_chunk chunk1(100, 100));
    EXPECT_NO_THROW(memory_chunk chunk2(1000, 1000));
    EXPECT_NO_THROW(memory_chunk chunk3(10000, 10000));
}

TEST(test_memory_chunk, chunk_size_and_count)
{
    memory_chunk chunk(100, 101);

    EXPECT_EQ(chunk.get_chunk_size(), 100);
    EXPECT_EQ(chunk.get_chunk_count(), 101);
}

TEST(test_memory_chunk, move_construction)
{
    memory_chunk chunk1(10, 12);
    memory_chunk chunk2 = std::move(chunk1);

    EXPECT_EQ(chunk1.get_chunk_size(), 0);
    EXPECT_EQ(chunk1.get_chunk_count(), 0);
    EXPECT_EQ(chunk2.get_chunk_size(), 10);
    EXPECT_EQ(chunk2.get_chunk_count(), 12);

    memory_chunk chunk3(13, 14);
    chunk3 = std::move(chunk2);

    EXPECT_EQ(chunk2.get_chunk_size(), 0);
    EXPECT_EQ(chunk2.get_chunk_count(), 0);
    EXPECT_EQ(chunk3.get_chunk_size(), 10);
    EXPECT_EQ(chunk3.get_chunk_count(), 12);
}

TEST(test_memory_chunk, allocate_and_deallocate)
{
    memory_chunk chunk(4, 2);
    int* ptr1 = reinterpret_cast<int*>(chunk.allocate(4));
    EXPECT_NE(ptr1, nullptr);
    
    *ptr1 = 42;
    EXPECT_EQ(*ptr1, 42);
    
    int* ptr2 = reinterpret_cast<int*>(chunk.allocate(4));
    EXPECT_NE(ptr1, nullptr);
    
    *ptr2 = 43;
    EXPECT_EQ(*ptr2, 43);

    int* ptr3 = reinterpret_cast<int*>(chunk.allocate(4));
    EXPECT_EQ(ptr3, nullptr);

    chunk.deallocate(reinterpret_cast<uint8_t*>(ptr1), 4);
    ptr3 = reinterpret_cast<int*>(chunk.allocate(4));
    EXPECT_NE(ptr3, nullptr);

    chunk.deallocate(reinterpret_cast<uint8_t*>(ptr2), 4);
    chunk.deallocate(reinterpret_cast<uint8_t*>(ptr3), 4);
}

TEST(test_memory_chunk, belongs)
{
    memory_chunk chunk(4, 2);
    int* ptr1 = reinterpret_cast<int*>(chunk.allocate(4));
    EXPECT_NE(ptr1, nullptr);

    EXPECT_EQ(chunk.belongs(reinterpret_cast<uint8_t*>(ptr1)), true);

    int a = 5;
    EXPECT_EQ(chunk.belongs(reinterpret_cast<uint8_t*>(&a)), false);
}
