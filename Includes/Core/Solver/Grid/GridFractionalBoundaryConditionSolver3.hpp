// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER3_HPP
#define CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER3_HPP

#include <Core/Field/CustomVectorField.hpp>
#include <Core/Grid/CellCenteredScalarGrid.hpp>
#include <Core/Solver/Grid/GridBoundaryConditionSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief Fractional 3-D boundary condition solver for grids.
//!
//! This class constrains the velocity field by projecting the flow to the
//! signed-distance field representation of the collider. This implementation
//! should pair up with GridFractionalSinglePhasePressureSolver3 to provide
//! sub-grid resolution velocity projection.
//!
class GridFractionalBoundaryConditionSolver3
    : public GridBoundaryConditionSolver3
{
 public:
    //! Default constructor.
    GridFractionalBoundaryConditionSolver3() = default;

    //! Deleted copy constructor.
    GridFractionalBoundaryConditionSolver3(
        const GridFractionalBoundaryConditionSolver3&) = delete;

    //! Deleted move constructor.
    GridFractionalBoundaryConditionSolver3(
        GridFractionalBoundaryConditionSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridFractionalBoundaryConditionSolver3() override = default;

    //! Deleted copy assignment operator.
    GridFractionalBoundaryConditionSolver3& operator=(
        const GridFractionalBoundaryConditionSolver3&) = delete;

    //! Deleted move assignment operator.
    GridFractionalBoundaryConditionSolver3& operator=(
        GridFractionalBoundaryConditionSolver3&&) noexcept = delete;

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    void ConstrainVelocity(FaceCenteredGrid3* velocity,
                           unsigned int extrapolationDepth = 5) override;

    //! Returns the signed distance field of the collider.
    [[nodiscard]] ScalarField3Ptr GetColliderSDF() const override;

    //! Returns the velocity field of the collider.
    [[nodiscard]] VectorField3Ptr GetColliderVelocityField() const override;

 protected:
    //! Invoked when a new collider is set.
    void OnColliderUpdated(const Vector3UZ& gridSize,
                           const Vector3D& gridSpacing,
                           const Vector3D& gridOrigin) override;

 private:
    CellCenteredScalarGrid3Ptr m_colliderSDF;
    CustomVectorField3Ptr m_colliderVel;
};

//! Shared pointer type for the GridFractionalBoundaryConditionSolver3.
using GridFractionalBoundaryConditionSolver3Ptr =
    std::shared_ptr<GridFractionalBoundaryConditionSolver3>;
}  // namespace CubbyFlow

#endif