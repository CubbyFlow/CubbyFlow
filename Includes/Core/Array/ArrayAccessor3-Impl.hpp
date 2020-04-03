// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_ACCESSOR3_IMPL_HPP
#define CUBBYFLOW_ARRAY_ACCESSOR3_IMPL_HPP

#include <Core/Utils/Constants.hpp>
#include <Core/Utils/Parallel.hpp>

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
template <typename T>
ArrayAccessor<T, 3>::ArrayAccessor() : m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ArrayAccessor<T, 3>::ArrayAccessor(const Size3& size, T* const data)
{
    Reset(size, data);
}

template <typename T>
ArrayAccessor<T, 3>::ArrayAccessor(size_t width, size_t height, size_t depth,
                                   T* const data)
{
    Reset(width, height, depth, data);
}

template <typename T>
ArrayAccessor<T, 3>::ArrayAccessor(const ArrayAccessor& other)
{
    Set(other);
}

template <typename T>
void ArrayAccessor<T, 3>::Set(const ArrayAccessor& other)
{
    Reset(other.m_size, other.m_data);
}

template <typename T>
void ArrayAccessor<T, 3>::Reset(const Size3& size, T* const data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
void ArrayAccessor<T, 3>::Reset(size_t width, size_t height, size_t depth,
                                T* const data)
{
    Reset(Size3(width, height, depth), data);
}

template <typename T>
T& ArrayAccessor<T, 3>::At(size_t i)
{
    assert(i < Width() * Height() * Depth());
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 3>::At(size_t i) const
{
    assert(i < Width() * Height() * Depth());
    return m_data[i];
}

template <typename T>
T& ArrayAccessor<T, 3>::At(const Point3UI& pt)
{
    return At(pt.x, pt.y, pt.z);
}

template <typename T>
const T& ArrayAccessor<T, 3>::At(const Point3UI& pt) const
{
    return At(pt.x, pt.y, pt.z);
}

template <typename T>
T& ArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k)
{
    assert(i < Width() && j < Height() && k < Depth());
    return m_data[Index(i, j, k)];
}

template <typename T>
const T& ArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k) const
{
    assert(i < Width() && j < Height() && k < Depth());
    return m_data[Index(i, j, k)];
}

template <typename T>
T* const ArrayAccessor<T, 3>::begin() const
{
    return m_data;
}

template <typename T>
T* const ArrayAccessor<T, 3>::end() const
{
    return m_data + Width() * Height() * Depth();
}

template <typename T>
T* ArrayAccessor<T, 3>::begin()
{
    return m_data;
}

template <typename T>
T* ArrayAccessor<T, 3>::end()
{
    return m_data + Width() * Height() * Depth();
}

template <typename T>
Size3 ArrayAccessor<T, 3>::size() const
{
    return m_size;
}

template <typename T>
size_t ArrayAccessor<T, 3>::Width() const
{
    return m_size.x;
}

template <typename T>
size_t ArrayAccessor<T, 3>::Height() const
{
    return m_size.y;
}

template <typename T>
size_t ArrayAccessor<T, 3>::Depth() const
{
    return m_size.z;
}

template <typename T>
T* const ArrayAccessor<T, 3>::data() const
{
    return m_data;
}

template <typename T>
void ArrayAccessor<T, 3>::Swap(ArrayAccessor& other)
{
    std::swap(other.m_data, m_data);
    std::swap(other.m_size, m_size);
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 3>::ForEach(Callback func) const
{
    for (size_t k = 0; k < Depth(); ++k)
    {
        for (size_t j = 0; j < Height(); ++j)
        {
            for (size_t i = 0; i < Width(); ++i)
            {
                func(At(i, j, k));
            }
        }
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 3>::ForEachIndex(Callback func) const
{
    for (size_t k = 0; k < Depth(); ++k)
    {
        for (size_t j = 0; j < Height(); ++j)
        {
            for (size_t i = 0; i < Width(); ++i)
            {
                func(i, j, k);
            }
        }
    }
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 3>::ParallelForEach(Callback func)
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(), ZERO_SIZE, Depth(),
                [&](size_t i, size_t j, size_t k) { func(At(i, j, k)); });
}

template <typename T>
template <typename Callback>
void ArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(), ZERO_SIZE, Depth(),
                func);
}

template <typename T>
size_t ArrayAccessor<T, 3>::Index(const Point3UI& pt) const
{
    assert(pt.x < Width() && pt.y < Height() && pt.z < Depth());
    return pt.x + Width() * pt.y + Width() * Height() * pt.z;
}

template <typename T>
size_t ArrayAccessor<T, 3>::Index(size_t i, size_t j, size_t k) const
{
    assert(i < Width() && j < Height() && k < Depth());
    return i + Width() * j + Width() * Height() * k;
}

template <typename T>
T& ArrayAccessor<T, 3>::operator[](size_t i)
{
    return m_data[i];
}

template <typename T>
const T& ArrayAccessor<T, 3>::operator[](size_t i) const
{
    return m_data[i];
}

template <typename T>
T& ArrayAccessor<T, 3>::operator()(const Point3UI& pt)
{
    return m_data[Index(pt)];
}

template <typename T>
const T& ArrayAccessor<T, 3>::operator()(const Point3UI& pt) const
{
    return m_data[Index(pt)];
}

template <typename T>
T& ArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k)
{
    return m_data[Index(i, j, k)];
}

template <typename T>
const T& ArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k) const
{
    return m_data[Index(i, j, k)];
}

template <typename T>
ArrayAccessor<T, 3>& ArrayAccessor<T, 3>::operator=(const ArrayAccessor& other)
{
    Set(other);
    return *this;
}

template <typename T>
ArrayAccessor<T, 3>::operator ConstArrayAccessor<T, 3>() const
{
    return ConstArrayAccessor<T, 3>(*this);
}

template <typename T>
ConstArrayAccessor<T, 3>::ConstArrayAccessor() : m_data(nullptr)
{
    // Do nothing
}

template <typename T>
ConstArrayAccessor<T, 3>::ConstArrayAccessor(const Size3& size,
                                             const T* const data)
{
    m_size = size;
    m_data = data;
}

template <typename T>
ConstArrayAccessor<T, 3>::ConstArrayAccessor(size_t width, size_t height,
                                             size_t depth, const T* const data)
{
    m_size = Size3(width, height, depth);
    m_data = data;
}

template <typename T>
ConstArrayAccessor<T, 3>::ConstArrayAccessor(const ArrayAccessor<T, 3>& other)
{
    m_size = other.size();
    m_data = other.data();
}

template <typename T>
ConstArrayAccessor<T, 3>::ConstArrayAccessor(const ConstArrayAccessor& other)
{
    m_size = other.m_size;
    m_data = other.m_data;
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::At(size_t i) const
{
    assert(i < Width() * Height() * Depth());
    return m_data[i];
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::At(const Point3UI& pt) const
{
    return At(pt.x, pt.y, pt.z);
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k) const
{
    assert(i < Width());
    assert(j < Height());
    assert(k < Depth());
    return m_data[Index(i, j, k)];
}

template <typename T>
const T* const ConstArrayAccessor<T, 3>::begin() const
{
    return m_data;
}

template <typename T>
const T* const ConstArrayAccessor<T, 3>::end() const
{
    return m_data + Width() * Height() * Depth();
}

template <typename T>
Size3 ConstArrayAccessor<T, 3>::size() const
{
    return m_size;
}

template <typename T>
size_t ConstArrayAccessor<T, 3>::Width() const
{
    return m_size.x;
}

template <typename T>
size_t ConstArrayAccessor<T, 3>::Height() const
{
    return m_size.y;
}

template <typename T>
size_t ConstArrayAccessor<T, 3>::Depth() const
{
    return m_size.z;
}

template <typename T>
const T* const ConstArrayAccessor<T, 3>::data() const
{
    return m_data;
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 3>::ForEach(Callback func) const
{
    for (size_t k = 0; k < Depth(); ++k)
    {
        for (size_t j = 0; j < Height(); ++j)
        {
            for (size_t i = 0; i < Width(); ++i)
            {
                func(At(i, j, k));
            }
        }
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 3>::ForEachIndex(Callback func) const
{
    for (size_t k = 0; k < Depth(); ++k)
    {
        for (size_t j = 0; j < Height(); ++j)
        {
            for (size_t i = 0; i < Width(); ++i)
            {
                func(i, j, k);
            }
        }
    }
}

template <typename T>
template <typename Callback>
void ConstArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, Width(), ZERO_SIZE, Height(), ZERO_SIZE, Depth(),
                func);
}

template <typename T>
size_t ConstArrayAccessor<T, 3>::Index(const Point3UI& pt) const
{
    assert(pt.x < m_size.x && pt.y < m_size.y && pt.z < m_size.z);
    return pt.x + Width() * pt.y + Width() * Height() * pt.z;
}

template <typename T>
size_t ConstArrayAccessor<T, 3>::Index(size_t i, size_t j, size_t k) const
{
    assert(i < m_size.x && j < m_size.y && k < m_size.z);
    return i + Width() * j + Width() * Height() * k;
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::operator[](size_t i) const
{
    return m_data[i];
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::operator()(size_t i, size_t j,
                                              size_t k) const
{
    return m_data[Index(i, j, k)];
}

template <typename T>
const T& ConstArrayAccessor<T, 3>::operator()(const Point3UI& pt) const
{
    return m_data[Index(pt)];
}
}  // namespace CubbyFlow

#endif