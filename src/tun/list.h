#pragma once
#include <array>
#include <vector>

template <typename T, std::size_t size>
using Array = std::array<T, size>;
template <typename T>
using List = std::vector<T>;
