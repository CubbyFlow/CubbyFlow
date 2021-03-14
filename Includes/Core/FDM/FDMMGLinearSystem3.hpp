// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_HPP
#define CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_HPP

#include <Core/FDM/FDMLinearSystem3.hpp>
#include <Core/Utils/MG.hpp>

namespace CubbyFlow
{
//! Multigrid-style 3-D FDM matrix.
using FDMMGMatrix3 = MGMatrix<FDMBLAS3>;

//! Multigrid-style 3-D FDM vector.
using FDMMGVector3 = MGVector<FDMBLAS3>;

//! Multigrid-syle 3-D linear system.
struct FDMMGLinearSystem3
{
    //! Clears the linear system.
    void Clear();

    //! Returns the number of multigrid levels.
    [[nodiscard]] size_t GetNumberOfLevels() const;

    //! Resizes the system with the coarsest resolution and number of levels.
    void ResizeWithCoarsest(const Vector3UZ& coarsestResolution,
                            size_t numberOfLevels);

    //!
    //! \brief Resizes the system with the finest resolution and max number of
    //! levels.
    //!
    //! This function resizes the system with multiple levels until the
    //! resolution is divisible with 2^(level-1).
    //!
    //! \param finestResolution - The finest grid resolution.
    //! \param maxNumberOfLevels - Maximum number of multigrid levels.
    //!
    void ResizeWithFinest(const Vector3UZ& finestResolution,
                          size_t maxNumberOfLevels);

    //! The system matrix.
    FDMMGMatrix3 A;

    //! The solution vector.
    FDMMGVector3 x;

    //! The RHS vector.
    FDMMGVector3 b;
};

//! Multigrid utilities for 2-D FDM system.
class FDMMGUtils3
{
 public:
    //! Restricts given finer grid to the coarser grid.
    static void Restrict(const FDMVector3& finer, FDMVector3* coarser);

    //! Corrects given coarser grid to the finer grid.
    static void Correct(const FDMVector3& coarser, FDMVector3* finer);

    //! Resizes the array with the coarsest resolution and number of levels.
    template <typename T>
    static void ResizeArrayWithCoarsest(const Vector3UZ& coarsestResolution,
                                        size_t numberOfLevels,
                                        std::vector<Array3<T>>* levels);

    //!
    //! \brief Resizes the array with the finest resolution and max number of
    //! levels.
    //!
    //! This function resizes the system with multiple levels until the
    //! resolution is divisible with 2^(level-1).
    //!
    //! \param finestResolution - The finest grid resolution.
    //! \param maxNumberOfLevels - Maximum number of multigrid levels.
    //! \param levels - The array to resize.
    //!
    template <typename T>
    static void ResizeArrayWithFinest(const Vector3UZ& finestResolution,
                                      size_t maxNumberOfLevels,
                                      std::vector<Array3<T>>* levels);
};
}  // namespace CubbyFlow

#include <Core/FDM/FDMMGLinearSystem3-Impl.hpp>

#endif