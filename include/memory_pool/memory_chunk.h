#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace memory_pool
{

class memory_chunk
{
public:
    memory_chunk() noexcept;
    memory_chunk(std::size_t chunk_size, std::size_t count);
    memory_chunk(const memory_chunk&) = delete;
    memory_chunk(memory_chunk&&) noexcept;
    memory_chunk& operator=(const memory_chunk&) = delete;
    memory_chunk& operator=(memory_chunk&&) noexcept;
    ~memory_chunk() noexcept;

    bool belongs(std::uint8_t* pointer) const noexcept;
    [[nodiscard]] std::uint8_t* allocate(std::size_t bytes) noexcept;
    void deallocate(std::uint8_t* ptr, std::size_t bytes) noexcept;

    std::size_t get_chunk_size() const noexcept;
    std::size_t get_chunk_count() const noexcept;
    
private:
    void set_blocks_in_use(std::size_t index, std::size_t n) noexcept;
    void set_blocks_free(std::size_t index, std::size_t n) noexcept;
    std::size_t find_contiguous_blocks(std::size_t n) const noexcept;

    std::size_t m_data_size;
    std::size_t m_ledger_size;
    std::uint8_t* m_data;
    std::uint8_t* m_ledger;
    std::size_t m_chunk_size;
    std::size_t m_count;
};

}