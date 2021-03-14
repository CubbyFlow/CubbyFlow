// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SVD_HPP
#define CUBBYFLOW_SVD_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief Singular value decomposition (SVD).
//!
//! This function decompose the input matrix \p a to \p u * \p w * \p v^T.
//!
//! \tparam T Real-value type.
//!
//! \param a The input matrix to decompose.
//! \param u Left-most output matrix.
//! \param w The vector of singular values.
//! \param v Right-most output matrix.
//!
template <typename T>
void SVD(const MatrixMxN<T>& a, MatrixMxN<T>& u, VectorN<T>& w,
         MatrixMxN<T>& v);

//!
//! \brief Singular value decomposition (SVD).
//!
//! This function decompose the input matrix \p a to \p u * \p w * \p v^T.
//!
//! \tparam T Real-value type.
//!
//! \param a The input matrix to decompose.
//! \param u Left-most output matrix.
//! \param w The vector of singular values.
//! \param v Right-most output matrix.
//!
template <typename T, size_t M, size_t N>
void SVD(const Matrix<T, M, N>& a, Matrix<T, M, N>& u, Vector<T, N>& w,
         Matrix<T, N, N>& v);
}  // namespace CubbyFlow

#include <Core/Math/SVD-Impl.hpp>

#endif