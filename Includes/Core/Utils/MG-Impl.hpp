// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MULTI_GRID_IMPL_HPP
#define CUBBYFLOW_MULTI_GRID_IMPL_HPP

namespace CubbyFlow
{
namespace Internal
{
template <typename BlasType>
MGResult MGVCycle(const MGMatrix<BlasType>& A, MGParameters<BlasType>& params,
                  unsigned int currentLevel, MGVector<BlasType>* x,
                  MGVector<BlasType>* b, MGVector<BlasType>* buffer)
{
    // 1) Relax a few times on Ax = b, with arbitrary x
    params.relaxFunc(A[currentLevel], (*b)[currentLevel],
                     params.numberOfRestrictionIter, params.maxTolerance,
                     &((*x)[currentLevel]), &((*buffer)[currentLevel]));

    // 2) if currentLevel is the coarsest grid, goto 5)
    if (currentLevel < A.levels.size() - 1)
    {
        auto r = buffer;
        BlasType::Residual(A[currentLevel], (*x)[currentLevel],
                           (*b)[currentLevel], &(*r)[currentLevel]);
        params.restrictFunc((*r)[currentLevel], &(*b)[currentLevel + 1]);

        BlasType::Set(0.0, &(*x)[currentLevel + 1]);

        params.maxTolerance *= 0.5;
        // Solve Ae = r
        MGVCycle(A, params, currentLevel + 1, x, b, buffer);
        params.maxTolerance *= 2.0;

        // 3) correct
        params.correctFunc((*x)[currentLevel + 1], &(*x)[currentLevel]);

        // 4) relax nIter times on Ax = b, with initial guess x
        if (currentLevel > 0)
        {
            params.relaxFunc(A[currentLevel], (*b)[currentLevel],
                             params.numberOfCorrectionIter, params.maxTolerance,
                             &((*x)[currentLevel]), &((*buffer)[currentLevel]));
        }
        else
        {
            params.relaxFunc(A[currentLevel], (*b)[currentLevel],
                             params.numberOfFinalIter, params.maxTolerance,
                             &((*x)[currentLevel]), &((*buffer)[currentLevel]));
        }
    }
    else
    {
        // 5) solve directly with initial guess x
        params.relaxFunc(A[currentLevel], (*b)[currentLevel],
                         params.numberOfCoarsestIter, params.maxTolerance,
                         &((*x)[currentLevel]), &((*buffer)[currentLevel]));

        BlasType::Residual(A[currentLevel], (*x)[currentLevel],
                           (*b)[currentLevel], &(*buffer)[currentLevel]);
    }

    BlasType::Residual(A[currentLevel], (*x)[currentLevel], (*b)[currentLevel],
                       &(*buffer)[currentLevel]);

    MGResult result;
    result.lastResidualNorm = BlasType::L2Norm((*buffer)[currentLevel]);
    return result;
}
}  // namespace Internal

template <typename BlasType>
const typename BlasType::MatrixType& MGMatrix<BlasType>::operator[](
    size_t i) const
{
    return levels[i];
}

template <typename BlasType>
typename BlasType::MatrixType& MGMatrix<BlasType>::operator[](size_t i)
{
    return levels[i];
}

template <typename BlasType>
const typename BlasType::MatrixType& MGMatrix<BlasType>::Finest() const
{
    return levels.Front();
}

template <typename BlasType>
typename BlasType::MatrixType& MGMatrix<BlasType>::Finest()
{
    return levels.Front();
}

template <typename BlasType>
const typename BlasType::VectorType& MGVector<BlasType>::operator[](
    size_t i) const
{
    return levels[i];
}

template <typename BlasType>
typename BlasType::VectorType& MGVector<BlasType>::operator[](size_t i)
{
    return levels[i];
}

template <typename BlasType>
const typename BlasType::VectorType& MGVector<BlasType>::Finest() const
{
    return levels.Front();
}

template <typename BlasType>
typename BlasType::VectorType& MGVector<BlasType>::Finest()
{
    return levels.Front();
}

template <typename BlasType>
MGResult MGVCycle(const MGMatrix<BlasType>& A, MGParameters<BlasType> params,
                  MGVector<BlasType>* x, MGVector<BlasType>* b,
                  MGVector<BlasType>* buffer)
{
    return Internal::MGVCycle<BlasType>(A, params, 0u, x, b, buffer);
}
}  // namespace CubbyFlow

#endif