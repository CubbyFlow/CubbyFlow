// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_STD_ARRAY_HPP
#define CUBBYFLOW_CUDA_STD_ARRAY_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/Matrix/Matrix.hpp>

#include <array>

namespace CubbyFlow
{
template <typename T, size_t N>
class CUDAStdArray
{
 public:
    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;
    using Pointer = T*;
    using ConstPointer = const T*;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray();

    template <typename... Args>
    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray(ConstReference first, Args... rest);

    CUBBYFLOW_CUDA_HOST CUDAStdArray(const std::array<T, N>& other);

    CUBBYFLOW_CUDA_HOST CUDAStdArray(const Vector<T, N>& other);

    ~CUDAStdArray() = default;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray(const CUDAStdArray& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray(CUDAStdArray&& other) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray& operator=(
        const CUDAStdArray& other);

    CUBBYFLOW_CUDA_HOST_DEVICE CUDAStdArray& operator=(
        CUDAStdArray&& other) noexcept;

    CUBBYFLOW_CUDA_HOST_DEVICE void Fill(ConstReference val);

    CUBBYFLOW_CUDA_HOST Vector<T, N> ToVector() const;

    CUBBYFLOW_CUDA_HOST_DEVICE Reference operator[](size_t i);

    CUBBYFLOW_CUDA_HOST_DEVICE ConstReference operator[](size_t i) const;

    CUBBYFLOW_CUDA_HOST_DEVICE bool operator==(const CUDAStdArray& other) const;

    CUBBYFLOW_CUDA_HOST_DEVICE bool operator!=(const CUDAStdArray& other) const;

 private:
    template <typename... Args>
    CUBBYFLOW_CUDA_HOST_DEVICE void SetAt(size_t i, ConstReference first,
                                          Args... rest);

    template <typename... Args>
    CUBBYFLOW_CUDA_HOST_DEVICE void SetAt(size_t i, ConstReference first);

    T m_elements[N];
};
}  // namespace CubbyFlow

#include <Core/CUDA/CUDAStdArray-Impl.hpp>

#endif

#endif