// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MULTI_GRID_HPP
#define CUBBYFLOW_MULTI_GRID_HPP

#include <functional>

namespace CubbyFlow
{
//! Multi-grid matrix wrapper.
template <typename BlasType>
struct MGMatrix
{
    std::vector<typename BlasType::MatrixType> levels;
    const typename BlasType::MatrixType& operator[](size_t i) const;
    typename BlasType::MatrixType& operator[](size_t i);
    [[nodiscard]] const typename BlasType::MatrixType& Finest() const;
    [[nodiscard]] typename BlasType::MatrixType& Finest();
};

//! Multi-grid vector wrapper.
template <typename BlasType>
struct MGVector
{
    std::vector<typename BlasType::VectorType> levels;
    const typename BlasType::VectorType& operator[](size_t i) const;
    typename BlasType::VectorType& operator[](size_t i);
    [[nodiscard]] const typename BlasType::VectorType& Finest() const;
    [[nodiscard]] typename BlasType::VectorType& Finest();
};

//! Multi-grid relax function type.
template <typename BlasType>
using MGRelaxFunc = std::function<void(
    const typename BlasType::MatrixType& A,
    const typename BlasType::VectorType& b, unsigned int numberOfIterations,
    double maxTolerance, typename BlasType::VectorType* x,
    typename BlasType::VectorType* buffer)>;

//! Multi-grid restriction function type.
template <typename BlasType>
using MGRestrictFunc =
    std::function<void(const typename BlasType::VectorType& finer,
                       typename BlasType::VectorType* coarser)>;

//! Multi-grid correction function type.
template <typename BlasType>
using MGCorrectFunc =
    std::function<void(const typename BlasType::VectorType& coarser,
                       typename BlasType::VectorType* finer)>;

//! Multi-grid input parameter set.
template <typename BlasType>
struct MGParameters
{
    //! Max number of multi-grid levels.
    size_t maxNumberOfLevels = 1;

    //! Number of iteration at restriction step.
    unsigned int numberOfRestrictionIter = 5;

    //! Number of iteration at correction step.
    unsigned int numberOfCorrectionIter = 5;

    //! Number of iteration at coarsest step.
    unsigned int numberOfCoarsestIter = 20;

    //! Number of iteration at final step.
    unsigned int numberOfFinalIter = 20;

    //! Relaxation function such as Jacobi or Gauss-Seidel.
    MGRelaxFunc<BlasType> relaxFunc;

    //! Restrict function that maps finer to coarser grid.
    MGRestrictFunc<BlasType> restrictFunc;

    //! Correction function that maps coarser to finer grid.
    MGCorrectFunc<BlasType> correctFunc;

    //! Max error tolerance.
    double maxTolerance = 1e-9;
};

//! Multi-grid result type.
struct MGResult
{
    //! Lastly measured norm of residual.
    double lastResidualNorm;
};

//!
//! \brief Performs Multi-grid with V-cycle.
//!
//! For given linear system matrix \p A and RHS vector \p b, this function
//! computes the solution \p x using Multi-grid method with V-cycle.
//!
template <typename BlasType>
MGResult MGVCycle(const MGMatrix<BlasType>& A, MGParameters<BlasType> params,
                  MGVector<BlasType>* x, MGVector<BlasType>* b,
                  MGVector<BlasType>* buffer);
}  // namespace CubbyFlow

#include <Core/Utils/MG-Impl.hpp>

#endif