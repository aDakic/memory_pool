#pragma once

#include <tuple>
#include <array>
#include "memory_chunk.h"
#include <iostream>

struct memory_config
{
    std::size_t chunk_size;
    std::size_t chunk_count;
};

class memory_pool
{
public:
    template <typename... Configs, std::enable_if_t<std::is_same_v<Configs..., memory_config>>>
    memory_pool(const Configs... configs)
    {
        std::tuple<Configs...> mem_config(configs...);
        std::apply([this](memory_config& conf)
                    {std::cout << conf.chunk_size <<" " << conf.chunk_count << "\n";}, 
                    mem_config);

    }

    [[nodiscard]] std::uint8_t* allocate(std::size_t node_size, std::size_t count);
    void deallocate(std::uint8_t* ptr, std::size_t count);
private:
    static constexpr uint16_t m_max_chunk_number {100};
    std::array<memory_chunk, m_max_chunk_number> m_chunks;
};