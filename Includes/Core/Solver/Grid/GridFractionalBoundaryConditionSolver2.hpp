// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER2_HPP
#define CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER2_HPP

#include <Core/Field/CustomVectorField.hpp>
#include <Core/Grid/CellCenteredScalarGrid.hpp>
#include <Core/Solver/Grid/GridBoundaryConditionSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief Fractional 2-D boundary condition solver for grids.
//!
//! This class constrains the velocity field by projecting the flow to the
//! signed-distance field representation of the collider. This implementation
//! should pair up with GridFractionalSinglePhasePressureSolver2 to provide
//! sub-grid resolution velocity projection.
//!
class GridFractionalBoundaryConditionSolver2
    : public GridBoundaryConditionSolver2
{
 public:
    //! Default constructor.
    GridFractionalBoundaryConditionSolver2() = default;

    //! Deleted copy constructor.
    GridFractionalBoundaryConditionSolver2(
        const GridFractionalBoundaryConditionSolver2&) = delete;

    //! Deleted move constructor.
    GridFractionalBoundaryConditionSolver2(
        GridFractionalBoundaryConditionSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridFractionalBoundaryConditionSolver2() override = default;

    //! Deleted copy assignment operator.
    GridFractionalBoundaryConditionSolver2& operator=(
        const GridFractionalBoundaryConditionSolver2&) = delete;

    //! Deleted move assignment operator.
    GridFractionalBoundaryConditionSolver2& operator=(
        GridFractionalBoundaryConditionSolver2&&) noexcept = delete;

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    void ConstrainVelocity(FaceCenteredGrid2* velocity,
                           unsigned int extrapolationDepth = 5) override;

    //! Returns the signed distance field of the collider.
    [[nodiscard]] ScalarField2Ptr GetColliderSDF() const override;

    //! Returns the velocity field of the collider.
    [[nodiscard]] VectorField2Ptr GetColliderVelocityField() const override;

 protected:
    //! Invoked when a new collider is set.
    void OnColliderUpdated(const Vector2UZ& gridSize,
                           const Vector2D& gridSpacing,
                           const Vector2D& gridOrigin) override;

 private:
    CellCenteredScalarGrid2Ptr m_colliderSDF;
    CustomVectorField2Ptr m_colliderVel;
};

//! Shared pointer type for the GridFractionalBoundaryConditionSolver2.
using GridFractionalBoundaryConditionSolver2Ptr =
    std::shared_ptr<GridFractionalBoundaryConditionSolver2>;
}  // namespace CubbyFlow

#endif