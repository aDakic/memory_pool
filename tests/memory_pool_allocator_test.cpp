#include <gtest/gtest.h>
#include <vector>
#include <map>
#include <list>

#include "memory_pool/memory_pool_allocator.h"

using namespace memory_pool::literals;

TEST(test_memory_pool_allocator, construction)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{sizeof(int), 100});

    EXPECT_NO_THROW(memory_pool::pool_allocator<int> alloc(pool));
}

TEST(test_memory_pool_allocator, vector)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{4_Kb, 1});
    memory_pool::pool_allocator<int> alloc(pool);

    std::vector<int, memory_pool::pool_allocator<int>> vec(alloc);

    EXPECT_NO_THROW(vec.resize(100));
    EXPECT_THROW(vec.resize(15000), std::bad_alloc);
}

TEST(test_memory_pool_allocator, map)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{5_byte, 100});

    memory_pool::pool_allocator<std::pair<int, int>> alloc(pool);

    std::map<int, int,
             std::less<int>, 
             memory_pool::pool_allocator<std::pair<const int, int>>> map(alloc);

    for (auto i = 0; i < 100; i++)
    {
        map.insert({i, i});
    }
}

struct A
{
    int a;
    int b;
    int c;
    int d;
};

TEST(test_memory_pool_allocator, multimap)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{7_byte, 100});

    memory_pool::pool_allocator<std::pair<int, A>> alloc(pool);

    std::multimap<int, A,
                   std::less<int>, 
                   memory_pool::pool_allocator<std::pair<const int, A>>> multimap(alloc);

    for (auto i = 0; i < 100; ++i)
    {
        multimap.insert({1, A()});
    }
}

TEST(test_memory_pool_allocator, vector_and_list)
{
    memory_pool::memory_pool pool(memory_pool::memory_config{4_Kb, 1}, 
                                  memory_pool::memory_config{5_byte, 100});
    memory_pool::pool_allocator<int> alloc(pool);

    std::vector<int, memory_pool::pool_allocator<int>> vec(alloc);
    std::list<int, memory_pool::pool_allocator<int>> list(alloc);
    EXPECT_NO_THROW(vec.resize(100));

    for (auto i = 0; i < 100; ++i)
        list.push_back(i);

    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), 99);
}
