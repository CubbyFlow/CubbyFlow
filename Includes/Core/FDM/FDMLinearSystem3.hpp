// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM3_HPP
#define CUBBYFLOW_FDM_LINEAR_SYSTEM3_HPP

#include <Core/Array/Array.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Matrix/MatrixCSR.hpp>

namespace CubbyFlow
{
//! The row of FDMMatrix3 where row corresponds to (i, j, k) grid point.
struct FDMMatrixRow3
{
    //! Diagonal component of the matrix (row, row).
    double center = 0.0;

    //! Off-diagonal element where column refers to (i+1, j, k) grid point.
    double right = 0.0;

    //! Off-diagonal element where column refers to (i, j+1, k) grid point.
    double up = 0.0;

    //! OFf-diagonal element where column refers to (i, j, k+1) grid point.
    double front = 0.0;
};

//! Vector type for 3-D finite differencing.
using FDMVector3 = Array3<double>;

//! Matrix type for 3-D finite differencing.
using FDMMatrix3 = Array3<FDMMatrixRow3>;

//! Linear system (Ax=b) for 3-D finite differencing.
struct FDMLinearSystem3
{
    //! Clears all the data.
    void Clear();

    //! Resizes the arrays with given grid size.
    void Resize(const Vector3UZ& size);

    //! System matrix.
    FDMMatrix3 A;

    //! Solution vector.
    FDMVector3 x;

    //! RHS vector.
    FDMVector3 b;
};

//! Compressed linear system (Ax=b) for 3-D finite differencing.
struct FDMCompressedLinearSystem3
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

//! BLAS operator wrapper for 3-D finite differencing.
struct FDMBLAS3
{
    using ScalarType = double;
    using VectorType = FDMVector3;
    using MatrixType = FDMMatrix3;

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

//! BLAS operator wrapper for compressed 3-D finite differencing.
struct FDMCompressedBLAS3
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