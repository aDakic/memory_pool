# Memory Pool

A popular approach to improve performance is to create a memory pool-based allocator. Instead of allocating memory every time an item is inserted or removed from a container, a large block of memory (the memory pool) is allocated beforehand, possibly at the startup of the program. The custom allocator will serve individual allocation requests by simply returning a pointer to memory from the pool. Actual deallocation of memory can be deferred until the lifetime of the memory pool ends.
In order to use memory pool memory model this library provides *memory_pool* and *pool_allocator* clases. Inspired by [foonathan memory library](https://memory.foonathan.net/)  and [CppCon speech: Practical memory pool based allocators for Modern C++](https://www.youtube.com/watch?v=l14Zkx5OXr4&t=312s)

## Usage

### Vector example

```c++
#include "memory_pool/memory_pool_allocator.h"

memory_pool pool(memory_config{4_Kb, 1});
pool_allocator<int> alloc(pool);

std::vector<int, pool_allocator<int>> vec(alloc);
```

### Map example
```c++
memory_pool pool(memory_config{5_byte, 100});

pool_allocator<std::pair<int, int>> alloc(pool);

std::map<int, int, std::less<int>, 
         pool_allocator<std::pair<const int, int>>> map(alloc);

    for (auto i = 0; i < 100; i++)
    {
        std::cout << i << std::endl;
        map.insert({i, i});
    }
```

### List and Vector example

```c++
memory_pool pool(memory_config{4_Kb, 1}, memory_config{5_byte, 100});
pool_allocator<int> alloc(pool);

std::vector<int, pool_allocator<int>> vec(alloc);
std::list<int, pool_allocator<int>> list(alloc);

vec.resize(100)
for (auto i = 0; i < 100; ++i)
    list.push_back(i);
```