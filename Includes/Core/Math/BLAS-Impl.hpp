// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BLAS_IMPL_HPP
#define CUBBYFLOW_BLAS_IMPL_HPP

#include <cmath>

namespace CubbyFlow
{
template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::Set(ScalarType s,
                                                   VectorType* result)
{
    result->Fill(s);
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::Set(const VectorType& v,
                                                   VectorType* result)
{
    result->CopyFrom(v);
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::Set(ScalarType s,
                                                   MatrixType* result)
{
    result->Fill(s);
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::Set(const MatrixType& m,
                                                   MatrixType* result)
{
    result->CopyFrom(m);
}

template <typename ScalarType, typename VectorType, typename MatrixType>
ScalarType BLAS<ScalarType, VectorType, MatrixType>::Dot(const VectorType& a,
                                                         const VectorType& b)
{
    return a.Dot(b);
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::AXPlusY(ScalarType a,
                                                       const VectorType& x,
                                                       const VectorType& y,
                                                       VectorType* result)
{
    *result = a * x + y;
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::MVM(const MatrixType& m,
                                                   const VectorType& v,
                                                   VectorType* result)
{
    *result = m * v;
}

template <typename ScalarType, typename VectorType, typename MatrixType>
void BLAS<ScalarType, VectorType, MatrixType>::Residual(const MatrixType& a,
                                                        const VectorType& x,
                                                        const VectorType& b,
                                                        VectorType* result)
{
    *result = b - a * x;
}

template <typename ScalarType, typename VectorType, typename MatrixType>
ScalarType BLAS<ScalarType, VectorType, MatrixType>::L2Norm(const VectorType& v)
{
    return std::sqrt(v.Dot(v));
}

template <typename ScalarType, typename VectorType, typename MatrixType>
ScalarType BLAS<ScalarType, VectorType, MatrixType>::LInfNorm(
    const VectorType& v)
{
    return std::fabs(v.AbsMax());
}
}  // namespace CubbyFlow

#endif