#include <cstring>
#include <cstdlib>
#include "memory_pool/memory_chunk.h"

memory_chunk::memory_chunk(std::size_t chunk_size, std::size_t count)
    : m_chunk_size(chunk_size), m_count(count)
{
    m_data_size = m_chunk_size * m_count;
    m_data = static_cast<std::uint8_t*>(std::malloc(m_data_size));

    m_ledger_size = 1 + ((m_count - 1) / 8);
    m_ledger = static_cast<std::uint8_t*>(std::malloc(m_ledger_size));

    std::memset(m_data, 0, m_data_size);
    std::memset(m_ledger, 0, m_ledger_size);
}

memory_chunk::~memory_chunk()
{
    std::free(m_data);
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
    {   
        return nullptr;  
    }  

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

void memory_chunk::set_blocks_in_use(std::size_t index, std::size_t n) noexcept
{
    for (std::size_t i = 0; i < n; ++i)
        *m_ledger ^= 1UL << (index + i);
    
}

void memory_chunk::set_blocks_free(std::size_t index, std::size_t n) noexcept
{
    for (std::size_t i = 0; i < n; ++i)
        *m_ledger &= ~(1UL << index + i);
}

std::size_t memory_chunk::find_contiguous_blocks(std::size_t n) const noexcept
{
    std::size_t blocks_num = 0;
    std::size_t index = -1;
    bool started_counting = false;

    for (std::size_t i = 0; i < m_ledger_size * 8; ++i)
    {
        const auto bit = *m_ledger & 1 << i;
        if(!bit)
        {
            if (!started_counting)
            {
                index = i;
                started_counting = true;
            }

            if(++blocks_num == n)
                break;
        }
        else
        {
            index = -1;
            blocks_num = 0;
            started_counting = 0;
        }
    }

    return index;
}