#include <cstring>
#include <cstdlib>
#include <utility>
#include "memory_pool/memory_chunk.h"
#include <iostream>
#include <algorithm>

namespace memory_pool
{

memory_chunk::memory_chunk() noexcept : 
    m_chunk_size(0), 
    m_count(0),
    m_data_size(0),
    m_data(nullptr),
    m_ledger_size(0),
    m_ledger(nullptr)
{}

memory_chunk::memory_chunk(std::size_t chunk_size, std::size_t count)
    : m_chunk_size(chunk_size), m_count(count)
{
    if (0 >= m_chunk_size || 0 >= m_count)
    {
        std::bad_alloc();
    }

    m_data_size = m_chunk_size * m_count;
    m_data = static_cast<std::uint8_t*>(std::malloc(m_data_size));

    m_ledger_size = m_data_size;
    m_ledger = static_cast<std::uint8_t*>(std::malloc(m_ledger_size));

    std::memset(m_data, 0, m_data_size);
    std::memset(m_ledger, 0, m_ledger_size);
}

memory_chunk::memory_chunk(memory_chunk&& other) noexcept : 
    m_chunk_size(std::exchange(other.m_chunk_size, 0)), 
    m_count(std::exchange(other.m_count, 0)),
    m_data_size(std::exchange(other.m_data_size, 0)),
    m_data(std::move(other.m_data)),
    m_ledger_size(std::exchange(other.m_ledger_size, 0)),
    m_ledger(std::move(other.m_ledger)) 
{
    other.m_data = nullptr;
    other.m_ledger = nullptr;
}

memory_chunk& memory_chunk::operator=(memory_chunk&& other) noexcept
{
    m_chunk_size = std::exchange(other.m_chunk_size, 0);
    m_count = std::exchange(other.m_count, 0);
    m_data_size = std::exchange(other.m_data_size, 0);
    m_data = std::move(other.m_data);
    m_ledger_size = std::exchange(other.m_ledger_size, 0);
    m_ledger = std::move(other.m_ledger);  

    other.m_data = nullptr;
    other.m_ledger = nullptr;

    return *this;
}

memory_chunk::~memory_chunk()
{
    if (m_data != nullptr)
        std::free(m_data);

    if (m_ledger != nullptr)
        std::free(m_ledger);
}

bool memory_chunk::belongs(std::uint8_t* pointer) const noexcept 
{
    return pointer >= m_data && pointer < m_data + m_count;
}

std::uint8_t* memory_chunk::allocate(std::size_t bytes) noexcept 
{
    const auto n = 1 + ((bytes - 1) / m_chunk_size);  
    const auto index = find_contiguous_blocks(n);  
    
    if (index == m_count) 
        return nullptr;  

    set_blocks_in_use(index, n);  
    return m_data + (index * m_chunk_size);
}

void memory_chunk::deallocate(std::uint8_t * pointer, std::size_t bytes) noexcept 
{       
    const std::size_t dist = static_cast<std::size_t>(pointer - m_data);
    const auto index = dist / m_chunk_size;
    const auto n = 1 + ((bytes - 1) / m_chunk_size);

    set_blocks_free(index, n);
}

std::size_t memory_chunk::get_chunk_size() const noexcept
{
    return m_chunk_size;
}

std::size_t memory_chunk::get_chunk_count() const noexcept
{
    return m_count;
}

void memory_chunk::set_blocks_in_use(std::size_t index, std::size_t n) noexcept
{
    std::for_each_n(m_ledger + index, n, [](auto& bit){ bit = 1;}); 
}

void memory_chunk::set_blocks_free(std::size_t index, std::size_t n) noexcept
{
    std::for_each_n(m_ledger + index, n, [](auto& bit){ bit = 0;});
}

std::size_t memory_chunk::find_contiguous_blocks(std::size_t n) const noexcept
{
    auto it = std::search_n(m_ledger, m_ledger + m_ledger_size, n, 0);
    size_t index = it - m_ledger;
    return index;
}

}