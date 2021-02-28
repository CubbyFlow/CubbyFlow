// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_MG_LINEAR_SYSTEM2_HPP
#define CUBBYFLOW_FDM_MG_LINEAR_SYSTEM2_HPP

#include <Core/FDM/FDMLinearSystem2.hpp>
#include <Core/Utils/MG.hpp>

namespace CubbyFlow
{
//! Multigrid-style 2-D FDM matrix.
using FDMMGMatrix2 = MGMatrix<FDMBLAS2>;

//! Multigrid-style 2-D FDM vector.
using FDMMGVector2 = MGVector<FDMBLAS2>;

//! Multigrid-syle 2-D linear system.
struct FDMMGLinearSystem2
{
    //! Clears the linear system.
    void Clear();

    //! Returns the number of multigrid levels.
    [[nodiscard]] size_t GetNumberOfLevels() const;

    //! Resizes the system with the coarsest resolution and number of levels.
    void ResizeWithCoarsest(const Vector2UZ& coarsestResolution,
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
    void ResizeWithFinest(const Vector2UZ& finestResolution,
                          size_t maxNumberOfLevels);

    //! The system matrix.
    FDMMGMatrix2 A;

    //! The solution vector.
    FDMMGVector2 x;

    //! The RHS vector.
    FDMMGVector2 b;
};

//! Multigrid utilities for 2-D FDM system.
class FDMMGUtils2
{
 public:
    //! Restricts given finer grid to the coarser grid.
    static void Restrict(const FDMVector2& finer, FDMVector2* coarser);

    //! Corrects given coarser grid to the finer grid.
    static void Correct(const FDMVector2& coarser, FDMVector2* finer);

    //! Resizes the array with the coarsest resolution and number of levels.
    template <typename T>
    static void ResizeArrayWithCoarsest(const Vector2UZ& coarsestResolution,
                                        size_t numberOfLevels,
                                        std::vector<Array2<T>>* levels);

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
    static void ResizeArrayWithFinest(const Vector2UZ& finestResolution,
                                      size_t maxNumberOfLevels,
                                      std::vector<Array2<T>>* levels);
};
}  // namespace CubbyFlow

#include <Core/FDM/FDMMGLinearSystem2-Impl.hpp>

#endif