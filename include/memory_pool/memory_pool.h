#pragma once

#include <tuple>
#include <array>
#include "memory_chunk.h"
#include "memory_common_types.h"
#include <iostream>

class memory_pool
{
public:
    template <typename... Configs>
    memory_pool(const Configs... configs)
    {
        if (sizeof...(configs) > m_max_chunk_number)
        {
            throw std::bad_alloc();
        }

        std::size_t i = 0;
        ((m_chunks[i++] = {configs.chunk_size, configs.chunk_size}), ...);
    }

    [[nodiscard]] std::uint8_t* allocate(std::size_t node_size, std::size_t count);
    void deallocate(std::uint8_t* ptr, std::size_t count);
private:
    static constexpr uint16_t m_max_chunk_number {5};
    std::array<memory_chunk, m_max_chunk_number> m_chunks;
};