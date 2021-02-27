// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_UTILS_HPP
#define CUBBYFLOW_ARRAY_UTILS_HPP

#include <Core/Array/ArrayView.hpp>

namespace CubbyFlow
{
template <typename T, size_t N>
void Fill(ArrayView<T, N> a, const Vector<size_t, N>& begin,
          const Vector<size_t, N>& end, const T& val);

template <typename T, size_t N>
void Fill(ArrayView<T, N> a, const T& val);

template <typename T>
void Fill(ArrayView<T, 1> a, size_t begin, size_t end, const T& val);

template <typename T, typename U, size_t N>
void Copy(ArrayView<T, N> src, const Vector<size_t, N>& begin,
          const Vector<size_t, N>& end, ArrayView<U, N> dst);

template <typename T, typename U, size_t N>
void Copy(ArrayView<T, N> src, ArrayView<U, N> dst);

template <typename T, typename U>
void Copy(ArrayView<T, 1> src, size_t begin, size_t end, ArrayView<U, 1> dst);

//!
//! \brief Extrapolates 2-D input data from 'valid' (1) to 'invalid' (0) region.
//!
//! This function extrapolates 2-D input data from 'valid' (1) to 'invalid' (0)
//! region. It iterates multiple times to propagate the 'valid' values to nearby
//! 'invalid' region. The maximum distance of the propagation is equal to
//! numberOfIterations. The input parameters 'valid' and 'data' should be
//! collocated.
//!
//! \param input - data to extrapolate
//! \param valid - set 1 if valid, else 0.
//! \param numberOfIterations - number of iterations for propagation
//! \param output - extrapolated output
//!
template <typename T, typename U>
void ExtrapolateToRegion(ArrayView2<T> input, ArrayView2<char> valid,
                         unsigned int numberOfIterations, ArrayView2<U> output);

//!
//! \brief Extrapolates 3-D input data from 'valid' (1) to 'invalid' (0) region.
//!
//! This function extrapolates 3-D input data from 'valid' (1) to 'invalid' (0)
//! region. It iterates multiple times to propagate the 'valid' values to nearby
//! 'invalid' region. The maximum distance of the propagation is equal to
//! numberOfIterations. The input parameters 'valid' and 'data' should be
//! collocated.
//!
//! \param input - data to extrapolate
//! \param valid - set 1 if valid, else 0.
//! \param numberOfIterations - number of iterations for propagation
//! \param output - extrapolated output
//!
template <typename T, typename U>
void ExtrapolateToRegion(ArrayView3<T> input, ArrayView3<char> valid,
                         unsigned int numberOfIterations, ArrayView3<U> output);
}  // namespace CubbyFlow

#include <Core/Array/ArrayUtils-Impl.hpp>

#endif