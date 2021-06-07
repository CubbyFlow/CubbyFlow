// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_ARRAY_HPP
#define CUBBYFLOW_CUDA_ARRAY_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/Array/Array.hpp>
#include <Core/CUDA/CUDAArrayBase.hpp>

namespace CubbyFlow
{
template <typename T, size_t N>
class CUDAArrayView;

template <typename T, size_t N>
class CUDAArray final : public CUDAArrayBase<T, N, Array<T, N>>
{
    using Base = CUDAArrayBase<T, N, Array<T, N>>;
    using Base::m_size;
    using Base::SetPtrAndSize;
    using Base::SwapPtrAndSize;

 public:
    using Base::At;
    using Base::ClearPtrAndSize;
    using Base::data;
    using Base::Length;

    CUDAArray();

    CUDAArray(const CUDAStdArray<size_t, N>& size, const T& initVal = T{});

    template <typename... Args>
    CUDAArray(size_t nx, Args... args);

    CUDAArray(NestedInitializerListsT<T, N> lst);

    template <size_t M = N>
    CUDAArray(const std::enable_if_t<(M == 1), std::vector<T>>& vec);

    template <typename OtherDerived>
    CUDAArray(const ArrayBase<T, N, OtherDerived>& other);

    template <typename OtherDerived>
    CUDAArray(const CUDAArrayBase<T, N, OtherDerived>& other);

    CUDAArray(const CUDAArray& other);

    CUDAArray(CUDAArray&& other) noexcept;

    ~CUDAArray() = default;

    template <size_t M = N>
    CUDAArray& operator=(const std::enable_if_t<(M == 1), std::vector<T>>& vec);

    template <typename OtherDerived>
    CUDAArray& operator=(const ArrayBase<T, N, OtherDerived>& other);

    template <typename OtherDerived>
    CUDAArray& operator=(const ArrayBase<const T, N, OtherDerived>& other);

    template <typename OtherDerived>
    CUDAArray& operator=(const CUDAArrayBase<T, N, OtherDerived>& other);

    template <typename OtherDerived>
    CUDAArray& operator=(const CUDAArrayBase<const T, N, OtherDerived>& other);

    CUDAArray& operator=(const CUDAArray& other);

    CUDAArray& operator=(CUDAArray&& other) noexcept;

    template <typename A, size_t M = N>
    std::enable_if_t<(M == 1), void> CopyFrom(const std::vector<T, A>& vec);

    template <typename OtherDerived>
    void CopyFrom(const ArrayBase<T, N, OtherDerived>& other);

    template <typename OtherDerived>
    void CopyFrom(const ArrayBase<const T, N, OtherDerived>& other);

    template <typename OtherDerived>
    void CopyFrom(const CUDAArrayBase<T, N, OtherDerived>& other);

    template <typename OtherDerived>
    void CopyFrom(const CUDAArrayBase<const T, N, OtherDerived>& other);

    template <typename A, size_t M = N>
    std::enable_if_t<(M == 1), void> CopyTo(std::vector<T, A>& vec);

    void CopyTo(Array<T, N>& other);

    void CopyTo(ArrayView<T, N>& other);

    void CopyTo(CUDAArray<T, N>& other);

    void CopyTo(CUDAArrayView<T, N>& other);

    void Fill(const T& val);

    void Resize(CUDAStdArray<size_t, N> size_, const T& initVal = T{});

    template <typename... Args>
    void Resize(size_t nx, Args... args);

    template <size_t M = N>
    std::enable_if_t<(M == 1), void> Append(const T& val);

    template <typename A, size_t M = N>
    std::enable_if_t<(M == 1), void> Append(const std::vector<T, A>& extra);

    template <typename OtherDerived, size_t M = N>
    std::enable_if_t<(M == 1), void> Append(
        const ArrayBase<T, N, OtherDerived>& extra);

    template <typename OtherDerived, size_t M = N>
    std::enable_if_t<(M == 1), void> Append(
        const CUDAArrayBase<T, N, OtherDerived>& extra);

    void Clear();

    void Swap(CUDAArray& other);

    CUDAArrayView<T, N> View();

    CUDAArrayView<const T, N> View() const;

 private:
    CUDAStdVector<T> m_data;
};

template <class T>
using CUDAArray1 = CUDAArray<T, 1>;

template <class T>
using CUDAArray2 = CUDAArray<T, 2>;

template <class T>
using CUDAArray3 = CUDAArray<T, 3>;

template <class T>
using CUDAArray4 = CUDAArray<T, 4>;
}  // namespace CubbyFlow

#include <Core/CUDA/CUDAArray-Impl.hpp>

#endif

#endif