// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_VIEW_HPP
#define CUBBYFLOW_ARRAY_VIEW_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
template <typename T, size_t N, typename Derived>
class ArrayBase;

template <typename T, size_t N>
class Array;

template <typename T, size_t N>
class ArrayView final : public ArrayBase<T, N, ArrayView<T, N>>
{
    using Base = ArrayBase<T, N, ArrayView<T, N>>;
    using Base::At;
    using Base::m_size;
    using Base::SetPtrAndSize;

 public:
    ArrayView();

    ArrayView(T* ptr, const Vector<size_t, N>& size);

    template <size_t M = N>
    ArrayView(std::enable_if_t<(M == 1), T*> ptr, size_t size);

    ArrayView(Array<T, N>& other);

    ~ArrayView() override = default;

    ArrayView(const ArrayView& other);

    ArrayView(ArrayView&& other) noexcept;

    ArrayView& operator=(const ArrayView& other);

    ArrayView& operator=(ArrayView&& other) noexcept;

    void Set(Array<T, N>& other);

    void Set(const ArrayView& other);

    void Fill(const T& val);
};

template <typename T, size_t N>
class ArrayView<const T, N> final
    : public ArrayBase<const T, N, ArrayView<const T, N>>
{
    using Base = ArrayBase<const T, N, ArrayView<const T, N>>;
    using Base::m_size;
    using Base::SetPtrAndSize;

 public:
    ArrayView();

    ArrayView(const T* ptr, const Vector<size_t, N>& size);

    template <size_t M = N>
    ArrayView(std::enable_if_t<(M == 1), T*> ptr, size_t size);

    ArrayView(const Array<T, N>& other);

    ~ArrayView() override = default;

    ArrayView(const ArrayView<T, N>& other);

    ArrayView(const ArrayView<const T, N>& other);

    ArrayView(ArrayView<const T, N>&&) noexcept;

    ArrayView& operator=(const ArrayView<T, N>& other);

    ArrayView& operator=(const ArrayView<const T, N>& other);

    ArrayView& operator=(ArrayView<const T, N>&& other) noexcept;

    void Set(const Array<T, N>& other);

    void Set(const ArrayView<T, N>& other);

    void Set(const ArrayView<const T, N>& other);
};

template <class T>
using ArrayView1 = ArrayView<T, 1>;

template <class T>
using ArrayView2 = ArrayView<T, 2>;

template <class T>
using ArrayView3 = ArrayView<T, 3>;

template <class T>
using ArrayView4 = ArrayView<T, 4>;

template <class T>
using ConstArrayView1 = ArrayView<const T, 1>;

template <class T>
using ConstArrayView2 = ArrayView<const T, 2>;

template <class T>
using ConstArrayView3 = ArrayView<const T, 3>;

template <class T>
using ConstArrayView4 = ArrayView<const T, 4>;
}  // namespace CubbyFlow

#include <Core/Array/ArrayView-Impl.hpp>

#endif