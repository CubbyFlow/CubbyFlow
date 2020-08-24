// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_IMPL_HPP
#define CUBBYFLOW_POINT_IMPL_HPP

#include <cassert>

namespace CubbyFlow
{
template <typename T, size_t N>
Point<T, N>::Point()
{
    for (auto& elem : m_elements)
    {
        elem = static_cast<T>(0);
    }
}

template <typename T, size_t N>
template <typename... Params>
Point<T, N>::Point(Params... params)
{
    static_assert(sizeof...(params) == N, "Invalid number of parameters.");

    SetAt(0, params...);
}

template <typename T, size_t N>
template <typename U>
Point<T, N>::Point(const std::initializer_list<U>& list)
{
    assert(list.size() >= N);

    size_t i = 0;
    for (const auto& inputElem : list)
    {
        m_elements[i] = static_cast<T>(inputElem);
        ++i;
    }
}

template <typename T, size_t N>
Point<T, N>::Point(const Point& other) : m_elements(other.m_elements)
{
    // Do nothing
}

template <typename T, size_t N>
template <typename U>
void Point<T, N>::Set(const std::initializer_list<U>& list)
{
    assert(list.size() >= N);

    size_t i = 0;
    for (const auto& inputElem : list)
    {
        m_elements[i] = static_cast<T>(inputElem);
        ++i;
    }
}

template <typename T, size_t N>
void Point<T, N>::Set(const Point& other)
{
    m_elements = other.m_elements;
}

template <typename T, size_t N>
template <typename U>
Point<T, N>& Point<T, N>::operator=(const std::initializer_list<U>& list)
{
    Set(list);
    return *this;
}

template <typename T, size_t N>
Point<T, N>& Point<T, N>::operator=(const Point& other)
{
    Set(other);
    return *this;
}

template <typename T, size_t N>
const T& Point<T, N>::operator[](size_t i) const
{
    return m_elements[i];
}

template <typename T, size_t N>
T& Point<T, N>::operator[](size_t i)
{
    return m_elements[i];
}

template <typename T, size_t N>
template <typename... Params>
void Point<T, N>::SetAt(size_t i, T v, Params... params)
{
    m_elements[i] = v;

    SetAt(i + 1, params...);
}

template <typename T, size_t N>
void Point<T, N>::SetAt(size_t i, T v)
{
    m_elements[i] = v;
}
}  // namespace CubbyFlow

#endif