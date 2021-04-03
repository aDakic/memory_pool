#pragma once

namespace memory_pool
{

struct memory_config
{
    std::size_t chunk_size;
    std::size_t chunk_count;
};

inline namespace literals
{

constexpr std::size_t operator"" _byte(unsigned long long value) noexcept
{
    return std::size_t(value * 8);
}

constexpr std::size_t operator"" _Kb(unsigned long long value) noexcept
{
    return std::size_t(value * 1024);
}

constexpr std::size_t operator"" _Mb(unsigned long long value) noexcept
{
    return std::size_t(value * 1024 * 1024);
}

constexpr std::size_t operator"" _Gb(unsigned long long value) noexcept
{
    return std::size_t(value * 1024 * 1024 * 1024);
}

}

}