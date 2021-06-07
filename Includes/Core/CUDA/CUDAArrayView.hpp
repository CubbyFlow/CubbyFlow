// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_ARRAY_VIEW_HPP
#define CUBBYFLOW_CUDA_ARRAY_VIEW_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAArray.hpp>

namespace CubbyFlow
{
template <typename T, size_t N>
class CUDAArrayView final : public CUDAArrayBase<T, N, CUDAArrayView<T, N>>
{
    using Base = CUDAArrayBase<T, N, CUDAArrayView<T, N>>;
    using Base::At;
    using Base::m_size;
    using Base::SetPtrAndSize;

 public:
    using Base::data;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView();

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(
        T* ptr, const CUDAStdArray<size_t, N>& size);

    template <size_t M = N>
    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(
        typename std::enable_if<(M == 1), T>::type* ptr, size_t size);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(CUDAArray<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(const CUDAArrayView& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(CUDAArrayView&& other) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView& operator=(
        const CUDAArrayView& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView& operator=(
        CUDAArrayView&& other) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE void Set(CUDAArray<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE void Set(const CUDAArrayView& other);

    CUBBYFLOW_CUDA_HOST void Fill(const T& val);
};

template <typename T, size_t N>
class CUDAArrayView<const T, N> final
    : public CUDAArrayBase<const T, N, CUDAArrayView<const T, N>>
{
    using Base = CUDAArrayBase<const T, N, CUDAArrayView<const T, N>>;
    using Base::m_size;
    using Base::SetPtrAndSize;

 public:
    using Base::data;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView();

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(
        const T* ptr, const CUDAStdArray<size_t, N>& size);

    template <size_t M = N>
    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(
        const typename std::enable_if<(M == 1), T>::type* ptr, size_t size);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(const CUDAArray<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(const CUDAArrayView<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(const CUDAArrayView& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView(CUDAArrayView&&) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView& operator=(
        const CUDAArrayView<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView& operator=(
        const CUDAArrayView& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAArrayView& operator=(
        CUDAArrayView&& other) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE void Set(const CUDAArray<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE void Set(const CUDAArrayView<T, N>& other);

    CUBBYFLOW_CUDA_HOST_DEVICE void Set(const CUDAArrayView& other);
};

template <class T>
using CUDAArrayView1 = CUDAArrayView<T, 1>;

template <class T>
using CUDAArrayView2 = CUDAArrayView<T, 2>;

template <class T>
using CUDAArrayView3 = CUDAArrayView<T, 3>;

template <class T>
using CUDAArrayView4 = CUDAArrayView<T, 4>;

template <class T>
using ConstCUDAArrayView1 = CUDAArrayView<const T, 1>;

template <class T>
using ConstCUDAArrayView2 = CUDAArrayView<const T, 2>;

template <class T>
using ConstCUDAArrayView3 = CUDAArrayView<const T, 3>;

template <class T>
using ConstCUDAArrayView4 = CUDAArrayView<const T, 4>;
}  // namespace CubbyFlow

#include <Core/CUDA/CUDAArrayView-Impl.hpp>

#endif

#endif