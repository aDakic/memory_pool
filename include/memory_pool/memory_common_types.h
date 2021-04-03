#pragma once

struct memory_config
{
    std::size_t chunk_size;
    std::size_t chunk_count;
};

constexpr std::size_t operator"" _B(unsigned long long value) noexcept
{
    return std::size_t(value * 8);
}

constexpr std::size_t operator"" _KiB(unsigned long long value) noexcept
{
    return std::size_t(value * 1024);
}

constexpr std::size_t operator"" _MiB(unsigned long long value) noexcept
{
    return std::size_t(value * 1024 * 1024);
}

constexpr std::size_t operator"" _GiB(unsigned long long value) noexcept
{
    return std::size_t(value * 1024 * 1024 * 1024);
}
