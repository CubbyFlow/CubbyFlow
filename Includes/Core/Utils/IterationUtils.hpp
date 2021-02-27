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
                  const Vector<IndexType, N>& end, Func func);

template <typename IndexType, typename Func>
void ForEachIndex(const Vector<IndexType, 1>& begin,
                  const Vector<IndexType, 1>& end, Func func);

template <typename IndexType, typename Func>
void ForEachIndex(IndexType begin, IndexType end, Func func);

template <typename IndexType, size_t N, typename Func>
void ForEachIndex(const Vector<IndexType, N>& size, Func func);

template <typename IndexType, typename Func>
void ForEachIndex(const Vector<IndexType, 1>& size, Func func);

template <typename IndexType, typename Func>
void ForEachIndex(IndexType size, Func func);

template <typename IndexType, size_t N, typename Func>
void ParallelForEachIndex(const Vector<IndexType, N>& begin,
                          const Vector<IndexType, N>& end, Func func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(const Vector<IndexType, 1>& begin,
                          const Vector<IndexType, 1>& end, Func func,
                          ExecutionPolicy policy);

template <typename IndexType, typename Func>
void ParallelForEachIndex(IndexType begin, IndexType end, Func func,
                          ExecutionPolicy policy);

template <typename IndexType, size_t N, typename Func>
void ParallelForEachIndex(const Vector<IndexType, N>& size, Func func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(const Vector<IndexType, 1>& size, Func func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);

template <typename IndexType, typename Func>
void ParallelForEachIndex(IndexType size, Func func,
                          ExecutionPolicy policy = ExecutionPolicy::Parallel);
}  // namespace CubbyFlow

#include <Core/Utils/IterationUtils-Impl.hpp>

#endif