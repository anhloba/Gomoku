#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <map>

struct Move
{
    int i;
    int j;
    int score;
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v);

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& arr);

