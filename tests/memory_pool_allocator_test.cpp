#include <gtest/gtest.h>
#include <vector>
#include <map>

#include "memory_pool/memory_pool_allocator.h"

TEST(test_memory_pool_allocator, construction)
{
    memory_pool pool(memory_config{sizeof(int), 100});

    EXPECT_NO_THROW(pool_allocator<int> alloc(pool));
}

TEST(test_memory_pool_allocator, vector)
{
    memory_pool pool(memory_config{4_Kb, 1});
    pool_allocator<int> alloc(pool);

    std::vector<int, pool_allocator<int>> vec(alloc);

    EXPECT_NO_THROW(vec.resize(100));
    EXPECT_THROW(vec.resize(15000), std::bad_alloc);
}

TEST(test_memory_pool_allocator, map)
{
    memory_pool pool(memory_config{5_byte, 100});

    pool_allocator<std::pair<int, int>> alloc(pool);

    std::map<int, int,
             std::less<int>, 
             pool_allocator<std::pair<const int, int>>> map(alloc);

    for (auto i = 0; i < 100; i++)
    {
        std::cout << i << std::endl;
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
    memory_pool pool(memory_config{7_byte, 100});

    pool_allocator<std::pair<int, A>> alloc(pool);

    std::multimap<int, A,
                   std::less<int>, 
                   pool_allocator<std::pair<const int, A>>> multimap(alloc);

    for (auto i = 0; i < 100; i++)
    {
        multimap.insert({1, A()});
    }
}
