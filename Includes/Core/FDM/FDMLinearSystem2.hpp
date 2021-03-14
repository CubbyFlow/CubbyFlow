// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM2_HPP
#define CUBBYFLOW_FDM_LINEAR_SYSTEM2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Matrix/MatrixCSR.hpp>

namespace CubbyFlow
{
//! The row of FDMMatrix2 where row corresponds to (i, j) grid point.
struct FDMMatrixRow2
{
    //! Diagonal component of the matrix (row, row).
    double center = 0.0;

    //! Off-diagonal element where column refers to (i+1, j) grid point.
    double right = 0.0;

    //! Off-diagonal element where column refers to (i, j+1) grid point.
    double up = 0.0;
};

//! Vector type for 2-D finite differencing.
using FDMVector2 = Array2<double>;

//! Matrix type for 2-D finite differencing.
using FDMMatrix2 = Array2<FDMMatrixRow2>;

//! Linear system (Ax=b) for 2-D finite differencing.
struct FDMLinearSystem2
{
    //! Clears all the data.
    void Clear();

    //! Resizes the arrays with given grid size.
    void Resize(const Vector2UZ& size);

    //! System matrix.
    FDMMatrix2 A;

    //! Solution vector.
    FDMVector2 x;

    //! RHS vector.
    FDMVector2 b;
};

//! Compressed linear system (Ax=b) for 2-D finite differencing.
struct FDMCompressedLinearSystem2
{
    //! Clears all the data.
    void Clear();

    //! System matrix.
    MatrixCSRD A;

    //! Solution vector.
    VectorND x;

    //! RHS vector.
    VectorND b;
};

//! BLAS operator wrapper for 2-D finite differencing.
struct FDMBLAS2
{
    using ScalarType = double;
    using VectorType = FDMVector2;
    using MatrixType = FDMMatrix2;

    //! Sets entire element of given vector \p result with scalar \p s.
    static void Set(ScalarType s, VectorType* result);

    //! Copies entire element of given vector \p result with other vector \p v.
    static void Set(const VectorType& v, VectorType* result);

    //! Sets entire element of given matrix \p result with scalar \p s.
    static void Set(ScalarType s, MatrixType* result);

    //! Copies entire element of given matrix \p result with other matrix \p v.
    static void Set(const MatrixType& m, MatrixType* result);

    //! Performs dot product with vector \p a and \p b.
    static double Dot(const VectorType& a, const VectorType& b);

    //! Performs ax + y operation where \p a is a matrix and \p x and \p y are
    //! vectors.
    static void AXPlusY(double a, const VectorType& x, const VectorType& y,
                        VectorType* result);

    //! Performs matrix-vector multiplication.
    static void MVM(const MatrixType& m, const VectorType& v,
                    VectorType* result);

    //! Computes residual vector (b - ax).
    static void Residual(const MatrixType& a, const VectorType& x,
                         const VectorType& b, VectorType* result);

    //! Returns L2-norm of the given vector \p v.
    [[nodiscard]] static ScalarType L2Norm(const VectorType& v);

    //! Returns Linf-norm of the given vector \p v.
    [[nodiscard]] static ScalarType LInfNorm(const VectorType& v);
};

//! BLAS operator wrapper for compressed 2-D finite differencing.
struct FDMCompressedBLAS2
{
    using ScalarType = double;
    using VectorType = VectorND;
    using MatrixType = MatrixCSRD;

    //! Sets entire element of given vector \p result with scalar \p s.
    static void Set(ScalarType s, VectorType* result);

    //! Copies entire element of given vector \p result with other vector \p v.
    static void Set(const VectorType& v, VectorType* result);

    //! Sets entire element of given matrix \p result with scalar \p s.
    static void Set(ScalarType s, MatrixType* result);

    //! Copies entire element of given matrix \p result with other matrix \p v.
    static void Set(const MatrixType& m, MatrixType* result);

    //! Performs dot product with vector \p a and \p b.
    static double Dot(const VectorType& a, const VectorType& b);

    //! Performs ax + y operation where \p a is a matrix and \p x and \p y are
    //! vectors.
    static void AXPlusY(double a, const VectorType& x, const VectorType& y,
                        VectorType* result);

    //! Performs matrix-vector multiplication.
    static void MVM(const MatrixType& m, const VectorType& v,
                    VectorType* result);

    //! Computes residual vector (b - ax).
    static void Residual(const MatrixType& a, const VectorType& x,
                         const VectorType& b, VectorType* result);

    //! Returns L2-norm of the given vector \p v.
    [[nodiscard]] static ScalarType L2Norm(const VectorType& v);

    //! Returns Linf-norm of the given vector \p v.
    [[nodiscard]] static ScalarType LInfNorm(const VectorType& v);
};
}  // namespace CubbyFlow

#endif