// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER2_HPP
#define CUBBYFLOW_GRID_BLOCKED_BOUNDARY_CONDITION_SOLVER2_HPP

#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief Blocked 2-D boundary condition solver for grids.
//!
//! This class constrains the velocity field by projecting the flow to the
//! blocked representation of the collider. A collider is rasterized into voxels
//! and each face of the collider voxels projects the velocity field onto its
//! face. This implementation should pair up with GridSinglePhasePressureSolver3
//! since the pressure solver assumes blocked boundary representation as well.
//!
class GridBlockedBoundaryConditionSolver2 final
    : public GridFractionalBoundaryConditionSolver2
{
 public:
    //! Default constructor.
    GridBlockedBoundaryConditionSolver2() = default;

    //! Deleted copy constructor.
    GridBlockedBoundaryConditionSolver2(
        const GridBlockedBoundaryConditionSolver2&) = delete;

    //! Deleted move constructor.
    GridBlockedBoundaryConditionSolver2(
        GridBlockedBoundaryConditionSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridBlockedBoundaryConditionSolver2() override = default;

    //! Deleted copy assignment operator.
    GridBlockedBoundaryConditionSolver2& operator=(
        const GridBlockedBoundaryConditionSolver2&) = delete;

    //! Deleted move assignment operator.
    GridBlockedBoundaryConditionSolver2& operator=(
        GridBlockedBoundaryConditionSolver2&&) noexcept = delete;

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    void ConstrainVelocity(FaceCenteredGrid2* velocity,
                           unsigned int extrapolationDepth = 5) override;

    //! Returns the marker which is 1 if occupied by the collider.
    [[nodiscard]] const Array2<char>& GetMarker() const;

 protected:
    //! Invoked when a new collider is set.
    void OnColliderUpdated(const Vector2UZ& gridSize,
                           const Vector2D& gridSpacing,
                           const Vector2D& gridOrigin) override;

 private:
    Array2<char> m_marker;
};

//! Shared pointer type for the GridBlockedBoundaryConditionSolver2.
using GridBlockedBoundaryConditionSolver2Ptr =
    std::shared_ptr<GridBlockedBoundaryConditionSolver2>;
}  // namespace CubbyFlow

#endif