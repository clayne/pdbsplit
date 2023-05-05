#pragma once

#include <cstdint>
#include <vector>

using offset_t = int64_t;
using va_t = uint64_t;

template <class T, class allocator = std::allocator<T>>
using vec_t = std::vector<T, allocator>;

#define GETTER(_x) __forceinline constexpr const decltype(m_##_x)& get_##_x() const noexcept { return m_##_x; }
