// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ITERATION_UTILS_HPP
#define CUBBYFLOW_ITERATION_UTILS_HPP

#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Parallel.hpp>

namespace CubbyFlow
{
template <typename IndexType, size_t N, typename Func>
void ForEachIndex(const Vector<IndexType, N>& begin,
                  const Vector<IndexType, N>& end, const Func& func);

template <typename IndexType, typename Func>
void ForEachIndex(const Vector<IndexType, 1>& begin,
                  const Vector<IndexType, 1>& end, const Func& func);

template <typename IndexType, typename Func>
void ForEachIndex(IndexType begin, IndexType end, const Func& func);

template <typename IndexType, size_t N, typename Func>
void ForEachIndex(const Vector<IndexType, N>& size, const Func& func);

template <typename IndexType, typename Func>
void ForEachIndex(const Vector<IndexType, 1>& size, const Func& func);

template <typename IndexType, typename Func>
void ForEachIndex(IndexType size, const Func& func);

template <typename IndexType, size_t N, typename Func>
void ParallelForEachIndex(const Vector<IndexType, N>& begin,
                          const Vector<IndexType, N>& end, const Func& func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(const Vector<IndexType, 1>& begin,
                          const Vector<IndexType, 1>& end, const Func& func,
                          ExecutionPolicy policy);

template <typename IndexType, typename Func>
void ParallelForEachIndex(IndexType begin, IndexType end, const Func& func,
                          ExecutionPolicy policy);

template <typename IndexType, size_t N, typename Func>
void ParallelForEachIndex(const Vector<IndexType, N>& size, const Func& func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(const Vector<IndexType, 1>& size, const Func& func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(IndexType size, const Func& func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

//! Unrolls vector-based indexing to size_t-based function.
template <typename ReturnType>
std::function<ReturnType(size_t)> Unroll1(
    const std::function<ReturnType(const Vector1UZ&)>& func)
{
    return [func](size_t i) { return func(Vector1UZ{ i }); };
}

//! Unrolls vector-based indexing to size_t-based function.
template <typename ReturnType>
std::function<ReturnType(size_t, size_t)> Unroll2(
    const std::function<ReturnType(const Vector2UZ&)>& func)
{
    return [func](size_t i, size_t j) { return func(Vector2UZ{ i, j }); };
}

//! Unrolls vector-based DataPositionFunc indexing to size_t-based function.
template <typename ReturnType>
std::function<ReturnType(size_t, size_t, size_t)> Unroll3(
    const std::function<ReturnType(const Vector3UZ&)>& func)
{
    return [func](size_t i, size_t j, size_t k) {
        return func(Vector3UZ{ i, j, k });
    };
}

template <typename ReturnType, size_t N>
struct GetUnroll
{
    // Do nothing
};

template <typename ReturnType>
struct GetUnroll<ReturnType, 1>
{
    static std::function<ReturnType(size_t)> Unroll(
        const std::function<ReturnType(const Vector1UZ&)>& func)
    {
        return [func](size_t i) { return func(Vector1UZ{ i }); };
    }
};

template <>
struct GetUnroll<void, 1>
{
    static std::function<void(size_t)> Unroll(
        const std::function<void(const Vector1UZ&)>& func)
    {
        return [func](size_t i) { func(Vector1UZ{ i }); };
    }
};

template <typename ReturnType>
struct GetUnroll<ReturnType, 2>
{
    static std::function<ReturnType(size_t, size_t)> Unroll(
        const std::function<ReturnType(const Vector2UZ&)>& func)
    {
        return [func](size_t i, size_t j) { return func(Vector2UZ{ i, j }); };
    }
};

template <>
struct GetUnroll<void, 2>
{
    static std::function<void(size_t, size_t)> Unroll(
        const std::function<void(const Vector2UZ&)>& func)
    {
        return [func](size_t i, size_t j) { func(Vector2UZ{ i, j }); };
    }
};

template <typename ReturnType>
struct GetUnroll<ReturnType, 3>
{
    static std::function<ReturnType(size_t, size_t, size_t)> Unroll(
        const std::function<ReturnType(const Vector3UZ&)>& func)
    {
        return [func](size_t i, size_t j, size_t k) {
            return func(Vector3UZ{ i, j, k });
        };
    }
};

template <>
struct GetUnroll<void, 3>
{
    static std::function<void(size_t, size_t, size_t)> Unroll(
        const std::function<void(const Vector3UZ&)>& func)
    {
        return [func](size_t i, size_t j, size_t k) {
            return func(Vector3UZ{ i, j, k });
        };
    }
};
}  // namespace CubbyFlow

#include <Core/Utils/IterationUtils-Impl.hpp>

#endif