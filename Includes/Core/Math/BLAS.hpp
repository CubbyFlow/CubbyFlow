// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BLAS_HPP
#define CUBBYFLOW_BLAS_HPP

namespace CubbyFlow
{
//!
//! \brief Generic BLAS operator wrapper class
//!
//! This class provides BLAS (Basic Linear Algebra Subprograms)-like set of
//! operators for vector and matrix class. By default, it supports Vector<T, 2>,
//! Vector<T, 3>, Vector<T, 4>, Matrix<T, 2, 2>, Matrix<T, 3, 3> and
//! Matrix<T, 4, 4>. For custom vector/matrix classes, create a new BLAS class
//! that conforms the function interfaces defined in this class. It will enable
//! performing linear algebra routines (such as conjugate gradient) for the
//! custom vector/matrix types.
//!
template <typename S, typename V, typename M>
struct BLAS
{
    using ScalarType = S;
    using VectorType = V;
    using MatrixType = M;

    //! Sets entire element of given vector \p result with scalar \p s.
    static void Set(ScalarType s, VectorType* result);

    //! Copies entire element of given vector \p result with other vector \p v.
    static void Set(const VectorType& v, VectorType* result);

    //! Sets entire element of given matrix \p result with scalar \p s.
    static void Set(ScalarType s, MatrixType* result);

    //! Copies entire element of given matrix \p result with other matrix \p v.
    static void Set(const MatrixType& m, MatrixType* result);

    //! Performs dot product with vector \p a and \p b.
    [[nodiscard]] static ScalarType Dot(const VectorType& a,
                                        const VectorType& b);

    //! Performs ax + y operation where \p a is a matrix and \p x and \p y are
    //! vectors.
    static void AXPlusY(ScalarType a, const VectorType& x, const VectorType& y,
                        VectorType* result);

    //! Performs matrix-vector multiplication.
    static void MVM(const MatrixType& m, const VectorType& v,
                    VectorType* result);

    //! Computes residual vector (b - ax).
    static void Residual(const MatrixType& a, const VectorType& x,
                         const VectorType& b, VectorType* result);

    //! Returns L2-norm of the given vector \p v.
    [[nodiscard]] static ScalarType L2Norm(const VectorType& v);

    //! Returns L-inf-norm of the given vector \p v.
    [[nodiscard]] static ScalarType LInfNorm(const VectorType& v);
};
}  // namespace CubbyFlow

#include <Core/Math/BLAS-Impl.hpp>

#endif