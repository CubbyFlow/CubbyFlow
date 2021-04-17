// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LEVEL_SET_SOLVER3_HPP
#define CUBBYFLOW_LEVEL_SET_SOLVER3_HPP

#include <Core/Grid/CollocatedVectorGrid.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Grid/ScalarGrid.hpp>

#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 3-D level set solver.
class LevelSetSolver3
{
 public:
    //! Default constructor.
    LevelSetSolver3() = default;

    //! Deleted copy constructor.
    LevelSetSolver3(const LevelSetSolver3&) = delete;

    //! Deleted move constructor.
    LevelSetSolver3(LevelSetSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    virtual ~LevelSetSolver3() = default;

    //! Deleted copy assignment operator.
    LevelSetSolver3& operator=(const LevelSetSolver3&) = delete;

    //! Deleted move assignment operator.
    LevelSetSolver3& operator=(LevelSetSolver3&&) noexcept = delete;

    //!
    //! Reinitializes given scalar field to signed-distance field.
    //!
    //! \param inputSDF Input signed-distance field which can be distorted.
    //! \param maxDistance Max range of reinitialization.
    //! \param outputSDF Output signed-distance field.
    //!
    virtual void Reinitialize(const ScalarGrid3& inputSDF, double maxDistance,
                              ScalarGrid3* outputSDF) = 0;

    //!
    //! Extrapolates given scalar field from negative to positive SDF region.
    //!
    //! \param input Input scalar field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output scalar field.
    //!
    virtual void Extrapolate(const ScalarGrid3& input, const ScalarField3& sdf,
                             double maxDistance, ScalarGrid3* output) = 0;

    //!
    //! Extrapolates given collocated vector field from negative to positive SDF
    //! region.
    //!
    //! \param input Input collocated vector field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output collocated vector field.
    //!
    virtual void Extrapolate(const CollocatedVectorGrid3& input,
                             const ScalarField3& sdf, double maxDistance,
                             CollocatedVectorGrid3* output) = 0;

    //!
    //! Extrapolates given face-centered vector field from negative to positive
    //! SDF region.
    //!
    //! \param input Input face-centered field to be extrapolated.
    //! \param sdf Reference signed-distance field.
    //! \param maxDistance Max range of extrapolation.
    //! \param output Output face-centered vector field.
    //!
    virtual void Extrapolate(const FaceCenteredGrid3& input,
                             const ScalarField3& sdf, double maxDistance,
                             FaceCenteredGrid3* output) = 0;
};

//! Shared pointer type for the LevelSetSolver3.
using LevelSetSolver3Ptr = std::shared_ptr<LevelSetSolver3>;
}  // namespace CubbyFlow

#endif