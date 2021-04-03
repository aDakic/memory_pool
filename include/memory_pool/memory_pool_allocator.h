#pragma once

#include "memory_pool/memory_pool.h"
#include <iostream>

template <typename T>
class pool_allocator
{
public:
    using value_type = T;
    
    pool_allocator(memory_pool& pool) : m_pool_ref(pool) {}

    template <class U> 
    constexpr pool_allocator (const pool_allocator<U>& alloc) noexcept : m_pool_ref(alloc.get_pool()) {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        T* ptr = reinterpret_cast<T*>(m_pool_ref.allocate(sizeof(T) * n));

        if (ptr == nullptr)
            throw std::bad_alloc();

        return ptr;
    }

    void deallocate(T* ptr, std::size_t n) noexcept
    {
        m_pool_ref.deallocate(reinterpret_cast<std::uint8_t*>(ptr), sizeof(T) * n);
    }
    
    memory_pool& get_pool() const noexcept { return m_pool_ref; }

private:
    memory_pool& m_pool_ref;
};