// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_ACCESSOR2_IMPL_HPP
#define CUBBYFLOW_ARRAY_ACCESSOR2_IMPL_HPP

#include <Core/Utils/Constants.hpp>
#include <Core/Utils/Parallel.hpp>

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
template <typename T>
ArrayAccessor<T, 2>::ArrayAccessor() : m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ArrayAccessor<T, 2>::ArrayAccessor(const Size2& size, T* data)
{
    Reset(size, data);
}

template <typename T>
ArrayAccessor<T, 2>::ArrayAccessor(size_t width, size_t height, T* data)
{
    Reset(width, height, data);
}

template <typename T>
ArrayAccessor<T, 2>::ArrayAccessor(const ArrayAccessor& other)
{
    Set(other);
}

template <typename T>
ArrayAccessor<T, 2>::ArrayAccessor(ArrayAccessor&& other) noexcept
{
    Set(other);
}

template <typename T>
void ArrayAccessor<T, 2>::Set(const ArrayAccessor& other)
{
    Reset(other.m_size, other.m_data);
}

template <typename T>
void ArrayAccessor<T, 2>::Reset(const Size2& size, T* data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
void ArrayAccessor<T, 2>::Reset(size_t width, size_t height, T* data)
{
    Reset(Size2(width, height), data);
}

template <typename T>
T& ArrayAccessor<T, 2>::At(size_t i)
{
    assert(i < Width() * Height());
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 2>::At(size_t i) const
{
    assert(i < Width() * Height());
    return m_data[i];
}

template <typename T>
T& ArrayAccessor<T, 2>::At(const Point2UI& pt)
{
    return At(pt.x, pt.y);
}

template <typename T>
const T& ArrayAccessor<T, 2>::At(const Point2UI& pt) const
{
    return At(pt.x, pt.y);
}

template <typename T>
T& ArrayAccessor<T, 2>::At(size_t i, size_t j)
{
    assert(i < Width() && j < Height());
    return m_data[Index(i, j)];
}

template <typename T>
const T& ArrayAccessor<T, 2>::At(size_t i, size_t j) const
{
    assert(i < Width() && j < Height());
    return m_data[Index(i, j)];
}

template <typename T>
T* ArrayAccessor<T, 2>::begin() const
{
    return m_data;
}

template <typename T>
T* ArrayAccessor<T, 2>::end() const
{
    return m_data + Width() * Height();
}

template <typename T>
T* ArrayAccessor<T, 2>::begin()
{
    return m_data;
}

template <typename T>
T* ArrayAccessor<T, 2>::end()
{
    return m_data + Width() * Height();
}

template <typename T>
Size2 ArrayAccessor<T, 2>::size() const
{
    return m_size;
}

template <typename T>
size_t ArrayAccessor<T, 2>::Width() const
{
    return m_size.x;
}

template <typename T>
size_t ArrayAccessor<T, 2>::Height() const
{
    return m_size.y;
}

template <typename T>
T* ArrayAccessor<T, 2>::data() const
{
    return m_data;
}

template <typename T>
void ArrayAccessor<T, 2>::Swap(ArrayAccessor& other)
{
    std::swap(other.m_data, m_data);
    std::swap(other.m_size, m_size);
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 2>::ForEach(Callback func) const
{
    for (size_t j = 0; j < Height(); ++j)
    {
        for (size_t i = 0; i < Width(); ++i)
        {
            func(At(i, j));
        }
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 2>::ForEachIndex(Callback func) const
{
    for (size_t j = 0; j < Height(); ++j)
    {
        for (size_t i = 0; i < Width(); ++i)
        {
            func(i, j);
        }
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 2>::ParallelForEach(Callback func)
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(),
                [&](size_t i, size_t j) { func(At(i, j)); });
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 2>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(), func);
}

template <typename T>
size_t ArrayAccessor<T, 2>::Index(const Point2UI& pt) const
{
    assert(pt.x < Width() && pt.y < Height());
    return pt.x + Width() * pt.y;
}

template <typename T>
size_t ArrayAccessor<T, 2>::Index(size_t i, size_t j) const
{
    assert(i < Width() && j < Height());
    return i + Width() * j;
}

template <typename T>
T& ArrayAccessor<T, 2>::operator[](size_t i)
{
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 2>::operator[](size_t i) const
{
    return m_data[i];
}

template <typename T>
T& ArrayAccessor<T, 2>::operator()(const Point2UI& pt)
{
    return m_data[Index(pt.x, pt.y)];
}

template <typename T>
const T& ArrayAccessor<T, 2>::operator()(const Point2UI& pt) const
{
    return m_data[Index(pt.x, pt.y)];
}

template <typename T>
T& ArrayAccessor<T, 2>::operator()(size_t i, size_t j)
{
    return m_data[Index(i, j)];
}

template <typename T>
const T& ArrayAccessor<T, 2>::operator()(size_t i, size_t j) const
{
    return m_data[Index(i, j)];
}

template <typename T>
ArrayAccessor<T, 2>& ArrayAccessor<T, 2>::operator=(const ArrayAccessor& other)
{
    Set(other);
    return *this;
}

template <typename T>
ArrayAccessor<T, 2>& ArrayAccessor<T, 2>::operator=(
    ArrayAccessor&& other) noexcept
{
    Set(other);
    return *this;
}

template <typename T>
ArrayAccessor<T, 2>::operator ConstArrayAccessor<T, 2>() const
{
    return ConstArrayAccessor<T, 2>(*this);
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor() : m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor(const Size2& size, const T* data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor(size_t width, size_t height,
                                             const T* data)
{
    m_size = Size2(width, height);
    m_data = data;
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor(const ArrayAccessor<T, 2>& other)
{
    m_size = other.size();
    m_data = other.data();
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor(const ConstArrayAccessor& other)
{
    m_size = other.m_size;
    m_data = other.m_data;
}

template <typename T>
ConstArrayAccessor<T, 2>::ConstArrayAccessor(
    ConstArrayAccessor&& other) noexcept
{
    m_size = other.m_size;
    m_data = other.m_data;
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::At(size_t i) const
{
    assert(i < Width() * Height());
    return m_data[i];
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::At(const Point2UI& pt) const
{
    return m_data[Index(pt)];
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::At(size_t i, size_t j) const
{
    assert(i < Width() && j < Height());
    return m_data[Index(i, j)];
}

template <typename T>
const T* ConstArrayAccessor<T, 2>::begin() const
{
    return m_data;
}

template <typename T>
const T* ConstArrayAccessor<T, 2>::end() const
{
    return m_data + Width() * Height();
}

template <typename T>
Size2 ConstArrayAccessor<T, 2>::size() const
{
    return m_size;
}

template <typename T>
size_t ConstArrayAccessor<T, 2>::Width() const
{
    return m_size.x;
}

template <typename T>
size_t ConstArrayAccessor<T, 2>::Height() const
{
    return m_size.y;
}

template <typename T>
const T* ConstArrayAccessor<T, 2>::data() const
{
    return m_data;
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 2>::ForEach(Callback func) const
{
    for (size_t j = 0; j < Height(); ++j)
    {
        for (size_t i = 0; i < Width(); ++i)
        {
            func(At(i, j));
        }
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 2>::ForEachIndex(Callback func) const
{
    for (size_t j = 0; j < Height(); ++j)
    {
        for (size_t i = 0; i < Width(); ++i)
        {
            func(i, j);
        }
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 2>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(), func);
}

template <typename T>
size_t ConstArrayAccessor<T, 2>::Index(const Point2UI& pt) const
{
    assert(pt.x < Width() && pt.y < Height());
    return pt.x + Width() * pt.y;
}

template <typename T>
size_t ConstArrayAccessor<T, 2>::Index(size_t i, size_t j) const
{
    assert(i < Width() && j < Height());
    return i + Width() * j;
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::operator[](size_t i) const
{
    return m_data[i];
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::operator()(const Point2UI& pt) const
{
    return m_data[Index(pt)];
}

template <typename T>
const T& ConstArrayAccessor<T, 2>::operator()(size_t i, size_t j) const
{
    return m_data[Index(i, j)];
}

template <typename T>
ConstArrayAccessor<T, 2>& ConstArrayAccessor<T, 2>::operator=(
    const ConstArrayAccessor& other)
{
    m_size = other.m_size;
    m_data = other.m_data;

    return *this;
}

template <typename T>
ConstArrayAccessor<T, 2>& ConstArrayAccessor<T, 2>::operator=(
    ConstArrayAccessor&& other) noexcept
{
    m_size = other.m_size;
    m_data = other.m_data;

    return *this;
}
}  // namespace CubbyFlow

#endif