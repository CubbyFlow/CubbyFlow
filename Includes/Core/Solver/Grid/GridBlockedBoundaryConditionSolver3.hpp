// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER3_HPP
#define CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER3_HPP

#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief Blocked 3-D boundary condition solver for grids.
//!
//! This class constrains the velocity field by projecting the flow to the
//! blocked representation of the collider. A collider is rasterized into voxels
//! and each face of the collider voxels projects the velocity field onto its
//! face. This implementation should pair up with GridSinglePhasePressureSolver3
//! since the pressure solver assumes blocked boundary representation as well.
//!
class GridBlockedBoundaryConditionSolver3 final
    : public GridFractionalBoundaryConditionSolver3
{
 public:
    //! Default constructor.
    GridBlockedBoundaryConditionSolver3() = default;

    //! Deleted copy constructor.
    GridBlockedBoundaryConditionSolver3(
        const GridBlockedBoundaryConditionSolver3&) = delete;

    //! Deleted move constructor.
    GridBlockedBoundaryConditionSolver3(
        GridBlockedBoundaryConditionSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridBlockedBoundaryConditionSolver3() override = default;

    //! Deleted copy assignment operator.
    GridBlockedBoundaryConditionSolver3& operator=(
        const GridBlockedBoundaryConditionSolver3&) = delete;

    //! Deleted move assignment operator.
    GridBlockedBoundaryConditionSolver3& operator=(
        GridBlockedBoundaryConditionSolver3&&) noexcept = delete;

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    void ConstrainVelocity(FaceCenteredGrid3* velocity,
                           unsigned int extrapolationDepth = 5) override;

    //! Returns the marker which is 1 if occupied by the collider.
    [[nodiscard]] const Array3<char>& GetMarker() const;

 protected:
    //! Invoked when a new collider is set.
    void OnColliderUpdated(const Vector3UZ& gridSize,
                           const Vector3D& gridSpacing,
                           const Vector3D& gridOrigin) override;

 private:
    Array3<char> m_marker;
};

//! Shared pointer type for the GridBlockedBoundaryConditionSolver3.
using GridBlockedBoundaryConditionSolver3Ptr =
    std::shared_ptr<GridBlockedBoundaryConditionSolver3>;
}  // namespace CubbyFlow

#endif