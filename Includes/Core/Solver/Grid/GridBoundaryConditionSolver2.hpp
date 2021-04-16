// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_BOUNDARY_CONDITION_SOLVER2_HPP
#define CUBBYFLOW_GRID_BOUNDARY_CONDITION_SOLVER2_HPP

#include <Core/Field/ScalarField.hpp>
#include <Core/Geometry/Collider.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D boundary condition solver for grids.
//!
//! This is a helper class to constrain the 2-D velocity field with given
//! collider object. It also determines whether to open any domain boundaries.
//! To control the friction level, tune the collider parameter.
//!
class GridBoundaryConditionSolver2
{
 public:
    //! Default constructor.
    GridBoundaryConditionSolver2() = default;

    //! Deleted copy constructor.
    GridBoundaryConditionSolver2(const GridBoundaryConditionSolver2&) = delete;

    //! Deleted move constructor.
    GridBoundaryConditionSolver2(GridBoundaryConditionSolver2&&) noexcept =
        delete;

    //! Default virtual destructor.
    virtual ~GridBoundaryConditionSolver2() = default;

    //! Deleted copy assignment operator.
    GridBoundaryConditionSolver2& operator=(
        const GridBoundaryConditionSolver2&) = delete;

    //! Deleted move assignment operator.
    GridBoundaryConditionSolver2& operator=(
        GridBoundaryConditionSolver2&&) noexcept = delete;

    //! Returns associated collider.
    [[nodiscard]] const Collider2Ptr& GetCollider() const;

    //!
    //! \brief Applies new collider and build the internals.
    //!
    //! This function is called to apply new collider and build the internal
    //! cache. To provide a hint to the cache, info for the expected velocity
    //! grid that will be constrained is provided.
    //!
    //! \param newCollider New collider to apply.
    //! \param gridSize Size of the velocity grid to be constrained.
    //! \param gridSpacing Grid spacing of the velocity grid to be constrained.
    //! \param gridOrigin Origin of the velocity grid to be constrained.
    //!
    void UpdateCollider(const Collider2Ptr& newCollider,
                        const Vector2UZ& gridSize, const Vector2D& gridSpacing,
                        const Vector2D& gridOrigin);

    //! Returns the closed domain boundary flag.
    [[nodiscard]] int GetClosedDomainBoundaryFlag() const;

    //! Sets the closed domain boundary flag.
    void SetClosedDomainBoundaryFlag(int flag);

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    virtual void ConstrainVelocity(FaceCenteredGrid2* velocity,
                                   unsigned int extrapolationDepth = 5) = 0;

    //! Returns the signed distance field of the collider.
    [[nodiscard]] virtual ScalarField2Ptr GetColliderSDF() const = 0;

    //! Returns the velocity field of the collider.
    [[nodiscard]] virtual VectorField2Ptr GetColliderVelocityField() const = 0;

 protected:
    //! Invoked when a new collider is set.
    virtual void OnColliderUpdated(const Vector2UZ& gridSize,
                                   const Vector2D& gridSpacing,
                                   const Vector2D& gridOrigin) = 0;

    //! Returns the size of the velocity grid to be constrained.
    [[nodiscard]] const Vector2UZ& GetGridSize() const;

    //! Returns the spacing of the velocity grid to be constrained.
    [[nodiscard]] const Vector2D& GetGridSpacing() const;

    //! Returns the origin of the velocity grid to be constrained.
    [[nodiscard]] const Vector2D& GetGridOrigin() const;

 private:
    Collider2Ptr m_collider;
    Vector2UZ m_gridSize;
    Vector2D m_gridSpacing;
    Vector2D m_gridOrigin;
    int m_closedDomainBoundaryFlag = DIRECTION_ALL;
};

//! Shared pointer type for the GridBoundaryConditionSolver2.
using GridBoundaryConditionSolver2Ptr =
    std::shared_ptr<GridBoundaryConditionSolver2>;
}  // namespace CubbyFlow

#endif