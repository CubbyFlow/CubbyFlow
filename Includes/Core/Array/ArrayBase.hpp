// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_BASE_HPP
#define CUBBYFLOW_ARRAY_BASE_HPP

#include <Core/Matrix/Matrix.hpp>

#include <algorithm>

namespace CubbyFlow
{
template <typename T, size_t N, typename DerivedArray>
class ArrayBase
{
 public:
    using Derived = DerivedArray;
    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;
    using Pointer = T*;
    using ConstPointer = const T*;
    using Iterator = T*;
    using ConstIterator = const T*;

    virtual ~ArrayBase() = default;

    size_t Index(size_t i) const;

    template <typename... Args>
    size_t Index(size_t i, Args... args) const;

    template <size_t... I>
    size_t Index(const Vector<size_t, N>& idx) const;

    Pointer data();

    ConstPointer data() const;

    const Vector<size_t, N>& Size() const;

    template <size_t M = N>
    std::enable_if_t<(M > 0), size_t> Width() const;

    template <size_t M = N>
    std::enable_if_t<(M > 1), size_t> Height() const;

    template <size_t M = N>
    std::enable_if_t<(M > 2), size_t> Depth() const;

    bool IsEmpty() const;

    size_t Length() const;

    Iterator begin();

    ConstIterator begin() const;

    Iterator end();

    ConstIterator end() const;

    Reference At(size_t i);

    ConstReference At(size_t i) const;

    template <typename... Args>
    Reference At(size_t i, Args... args);

    template <typename... Args>
    ConstReference At(size_t i, Args... args) const;

    Reference At(const Vector<size_t, N>& idx);

    ConstReference At(const Vector<size_t, N>& idx) const;

    Reference operator[](size_t i);

    ConstReference operator[](size_t i) const;

    template <typename... Args>
    Reference operator()(size_t i, Args... args);

    template <typename... Args>
    ConstReference operator()(size_t i, Args... args) const;

    Reference operator()(const Vector<size_t, N>& idx);

    ConstReference operator()(const Vector<size_t, N>& idx) const;

 protected:
    ArrayBase();

    ArrayBase(const ArrayBase& other);

    ArrayBase(ArrayBase&& other) noexcept;

    template <typename... Args>
    void SetPtrAndSize(Pointer ptr, size_t ni, Args... args);

    void SetPtrAndSize(Pointer data, Vector<size_t, N> size);

    void SwapPtrAndSize(ArrayBase& other);

    void ClearPtrAndSize();

    ArrayBase& operator=(const ArrayBase& other);

    ArrayBase& operator=(ArrayBase&& other) noexcept;

    Pointer m_ptr = nullptr;
    Vector<size_t, N> m_size;

 private:
    template <typename... Args>
    size_t IndexInternal(size_t d, size_t i, Args... args) const;

    size_t IndexInternal(size_t, size_t i) const;

    template <size_t... I>
    size_t IndexInternal(const Vector<size_t, N>& idx,
                         std::index_sequence<I...>) const;
};
}  // namespace CubbyFlow

#include <Core/Array/ArrayBase-Impl.hpp>

#endif