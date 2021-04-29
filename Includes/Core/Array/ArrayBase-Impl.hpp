// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_BASE_IMPL_HPP
#define CUBBYFLOW_ARRAY_BASE_IMPL_HPP

namespace CubbyFlow
{
template <typename T, size_t N, typename D>
size_t ArrayBase<T, N, D>::Index(size_t i) const
{
    return i;
}

template <typename T, size_t N, typename D>
template <typename... Args>
size_t ArrayBase<T, N, D>::Index(size_t i, Args... args) const
{
    static_assert(sizeof...(args) == N - 1, "Invalid number of indices.");

    return i + m_size[0] * IndexInternal(1, args...);
}

template <typename T, size_t N, typename D>
template <size_t... I>
size_t ArrayBase<T, N, D>::Index(const Vector<size_t, N>& idx) const
{
    return IndexInternal(idx, std::make_index_sequence<N>{});
}

template <typename T, size_t N, typename D>
T* ArrayBase<T, N, D>::data()
{
    return m_ptr;
}

template <typename T, size_t N, typename D>
const T* ArrayBase<T, N, D>::data() const
{
    return m_ptr;
}

template <typename T, size_t N, typename D>
const Vector<size_t, N>& ArrayBase<T, N, D>::Size() const
{
    return m_size;
}

template <typename T, size_t N, typename D>
template <size_t M>
std::enable_if_t<(M > 0), size_t> ArrayBase<T, N, D>::Width() const
{
    return m_size.x;
}

template <typename T, size_t N, typename D>
template <size_t M>
std::enable_if_t<(M > 1), size_t> ArrayBase<T, N, D>::Height() const
{
    return m_size.y;
}

template <typename T, size_t N, typename D>
template <size_t M>
std::enable_if_t<(M > 2), size_t> ArrayBase<T, N, D>::Depth() const
{
    return m_size.z;
}

template <typename T, size_t N, typename D>
bool ArrayBase<T, N, D>::IsEmpty() const
{
    return Length() == 0;
}

template <typename T, size_t N, typename D>
size_t ArrayBase<T, N, D>::Length() const
{
    return Product<size_t, N>(m_size, 1);
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::Iterator ArrayBase<T, N, D>::begin()
{
    return m_ptr;
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::ConstIterator ArrayBase<T, N, D>::begin() const
{
    return m_ptr;
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::Iterator ArrayBase<T, N, D>::end()
{
    return m_ptr + Length();
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::ConstIterator ArrayBase<T, N, D>::end() const
{
    return m_ptr + Length();
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::Iterator ArrayBase<T, N, D>::rbegin()
{
    return end() - 1;
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::ConstIterator ArrayBase<T, N, D>::rbegin() const
{
    return end() - 1;
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::Iterator ArrayBase<T, N, D>::rend()
{
    return begin() - 1;
}

template <typename T, size_t N, typename D>
typename ArrayBase<T, N, D>::ConstIterator ArrayBase<T, N, D>::rend() const
{
    return begin() - 1;
}

template <typename T, size_t N, typename D>
T& ArrayBase<T, N, D>::At(size_t i)
{
    return m_ptr[i];
}

template <typename T, size_t N, typename D>
const T& ArrayBase<T, N, D>::At(size_t i) const
{
    return m_ptr[i];
}

template <typename T, size_t N, typename D>
template <typename... Args>
T& ArrayBase<T, N, D>::At(size_t i, Args... args)
{
    return data()[Index(i, args...)];
}

template <typename T, size_t N, typename D>
template <typename... Args>
const T& ArrayBase<T, N, D>::At(size_t i, Args... args) const
{
    return m_ptr[Index(i, args...)];
}

template <typename T, size_t N, typename D>
T& ArrayBase<T, N, D>::At(const Vector<size_t, N>& idx)
{
    return data()[Index(idx)];
}

template <typename T, size_t N, typename D>
const T& ArrayBase<T, N, D>::At(const Vector<size_t, N>& idx) const
{
    return data()[Index(idx)];
}

template <typename T, size_t N, typename D>
T& ArrayBase<T, N, D>::operator[](size_t i)
{
    return At(i);
}

template <typename T, size_t N, typename D>
const T& ArrayBase<T, N, D>::operator[](size_t i) const
{
    return At(i);
}

template <typename T, size_t N, typename D>
template <typename... Args>
T& ArrayBase<T, N, D>::operator()(size_t i, Args... args)
{
    return At(i, args...);
}

template <typename T, size_t N, typename D>
template <typename... Args>
const T& ArrayBase<T, N, D>::operator()(size_t i, Args... args) const
{
    return At(i, args...);
}

template <typename T, size_t N, typename D>
T& ArrayBase<T, N, D>::operator()(const Vector<size_t, N>& idx)
{
    return At(idx);
}

template <typename T, size_t N, typename D>
const T& ArrayBase<T, N, D>::operator()(const Vector<size_t, N>& idx) const
{
    return At(idx);
}

template <typename T, size_t N, typename D>
ArrayBase<T, N, D>::ArrayBase() : m_size{}
{
    // Do nothing
}

template <typename T, size_t N, typename D>
ArrayBase<T, N, D>::ArrayBase(const ArrayBase& other)
{
    SetPtrAndSize(other.m_ptr, other.m_size);
}

template <typename T, size_t N, typename D>
ArrayBase<T, N, D>::ArrayBase(ArrayBase&& other) noexcept
{
    *this = std::move(other);
}

template <typename T, size_t N, typename D>
ArrayBase<T, N, D>& ArrayBase<T, N, D>::operator=(const ArrayBase& other)
{
    SetPtrAndSize(other.data(), other.Size());

    return *this;
}

template <typename T, size_t N, typename D>
ArrayBase<T, N, D>& ArrayBase<T, N, D>::operator=(ArrayBase&& other) noexcept
{
    SetPtrAndSize(other.data(), other.Size());
    other.SetPtrAndSize(nullptr, Vector<size_t, N>{});

    return *this;
}

template <typename T, size_t N, typename D>
template <typename... Args>
void ArrayBase<T, N, D>::SetPtrAndSize(T* ptr, size_t ni, Args... args)
{
    SetPtrAndSize(ptr, Vector<size_t, N>{ ni, args... });
}

template <typename T, size_t N, typename D>
void ArrayBase<T, N, D>::SetPtrAndSize(T* data, Vector<size_t, N> size)
{
    m_ptr = data;
    m_size = size;
}

template <typename T, size_t N, typename D>
void ArrayBase<T, N, D>::ClearPtrAndSize()
{
    SetPtrAndSize(nullptr, Vector<size_t, N>{});
}

template <typename T, size_t N, typename D>
void ArrayBase<T, N, D>::SwapPtrAndSize(ArrayBase& other)
{
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_size, other.m_size);
}

template <typename T, size_t N, typename D>
template <typename... Args>
size_t ArrayBase<T, N, D>::IndexInternal(size_t d, size_t i, Args... args) const
{
    return i + m_size[d] * IndexInternal(d + 1, args...);
}

template <typename T, size_t N, typename D>
size_t ArrayBase<T, N, D>::IndexInternal(size_t, size_t i) const
{
    return i;
}

template <typename T, size_t N, typename D>
template <size_t... I>
size_t ArrayBase<T, N, D>::IndexInternal(const Vector<size_t, N>& idx,
                                         std::index_sequence<I...>) const
{
    return Index(idx[I]...);
}
}  // namespace CubbyFlow

#endif