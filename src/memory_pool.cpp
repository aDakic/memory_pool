#include "memory_pool/memory_pool.h"

std::uint8_t* memory_pool::allocate(std::size_t bytes) noexcept
{
    std::uint8_t* ptr = nullptr;

    for (auto & chunk : m_chunks) 
    {    
        if (chunk.get_chunk_size() >= bytes)
        {
            if (ptr = chunk.allocate(bytes); ptr != nullptr) 
                break;
        }
    }

    return ptr;
}

void memory_pool::deallocate(std::uint8_t* ptr, std::size_t bytes) noexcept
{
    for (auto & chunk : m_chunks) 
    {
        if (chunk.belongs(ptr))
        {
            chunk.deallocate(ptr, bytes);
            ptr = nullptr;
            return;
        }
    }
}