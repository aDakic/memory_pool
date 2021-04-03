#pragma once

#include "memory_pool/memory_pool.h"

template <typename T>
class pool_allocator
{
public:
    using value_type = T;
    
    pool_allocator(memory_pool& pool) : m_pool_ref(pool) {}

    template <class U> 
    constexpr pool_allocator (const pool_allocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        return static_cast<T*>(m_pool_ref.allocate(sizeof(T) * n));
    }

    void deallocate(T* ptr, std::size_t n) noexcept
    {
        m_pool_ref.deallocate(ptr, sizeof(T) * n);
    }
    
private:
    memory_pool& m_pool_ref;
};