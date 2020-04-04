// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_ACCESSOR1_IMPL_HPP
#define CUBBYFLOW_ARRAY_ACCESSOR1_IMPL_HPP

#include <Core/Utils/Constants.hpp>
#include <Core/Utils/Parallel.hpp>

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
template <typename T>
ArrayAccessor<T, 1>::ArrayAccessor() : m_size(0), m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ArrayAccessor<T, 1>::ArrayAccessor(size_t size, T* const data)
{
    Reset(size, data);
}

template <typename T>
ArrayAccessor<T, 1>::ArrayAccessor(const ArrayAccessor& other)
{
    Set(other);
}

template <typename T>
void ArrayAccessor<T, 1>::Set(const ArrayAccessor& other)
{
    Reset(other.m_size, other.m_data);
}

template <typename T>
void ArrayAccessor<T, 1>::Reset(size_t size, T* const data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
T& ArrayAccessor<T, 1>::At(size_t i)
{
    assert(i < size());
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 1>::At(size_t i) const
{
    assert(i < size());
    return m_data[i];
}

template <typename T>
T* const ArrayAccessor<T, 1>::begin() const
{
    return m_data;
}

template <typename T>
T* const ArrayAccessor<T, 1>::end() const
{
    return m_data + m_size;
}

template <typename T>
T* ArrayAccessor<T, 1>::begin()
{
    return m_data;
}

template <typename T>
T* ArrayAccessor<T, 1>::end()
{
    return m_data + m_size;
}

template <typename T>
size_t ArrayAccessor<T, 1>::size() const
{
    return m_size;
}

template <typename T>
T* const ArrayAccessor<T, 1>::data() const
{
    return m_data;
}

template <typename T>
void ArrayAccessor<T, 1>::Swap(ArrayAccessor& other)
{
    std::swap(other.m_data, m_data);
    std::swap(other.m_size, m_size);
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 1>::ForEach(Callback func) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        func(At(i));
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 1>::ForEachIndex(Callback func) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        func(i);
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 1>::ParallelForEach(Callback func)
{
    ParallelFor(ZERO_SIZE, size(), [&](size_t i) { func(At(i)); });
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 1>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, size(), func);
}

template <typename T>
T& ArrayAccessor<T, 1>::operator[](size_t i)
{
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 1>::operator[](size_t i) const
{
    return m_data[i];
}

template <typename T>
ArrayAccessor<T, 1>& ArrayAccessor<T, 1>::operator=(const ArrayAccessor& other)
{
    Set(other);
    return *this;
}

template <typename T>
ArrayAccessor<T, 1>::operator ConstArrayAccessor<T, 1>() const
{
    return ConstArrayAccessor<T, 1>(*this);
}

template <typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor() : m_size(0), m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(size_t size, const T* const data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ArrayAccessor<T, 1>& other)
{
    m_size = other.size();
    m_data = other.data();
}

template <typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ConstArrayAccessor& other)
{
    m_size = other.m_size;
    m_data = other.m_data;
}

template <typename T>
const T& ConstArrayAccessor<T, 1>::At(size_t i) const
{
    assert(i < size());
    return m_data[i];
}

template <typename T>
const T* const ConstArrayAccessor<T, 1>::begin() const
{
    return m_data;
}

template <typename T>
const T* const ConstArrayAccessor<T, 1>::end() const
{
    return m_data + m_size;
}

template <typename T>
size_t ConstArrayAccessor<T, 1>::size() const
{
    return m_size;
}

template <typename T>
const T* const ConstArrayAccessor<T, 1>::data() const
{
    return m_data;
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 1>::ForEach(Callback func) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        func(At(i));
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 1>::ForEachIndex(Callback func) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        func(i);
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 1>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, size(), func);
}

template <typename T>
const T& ConstArrayAccessor<T, 1>::operator[](size_t i) const
{
    return m_data[i];
}
}  // namespace CubbyFlow

#endif