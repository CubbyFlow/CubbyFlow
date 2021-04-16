// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_PRESSURE_SOLVER2_HPP
#define CUBBYFLOW_GRID_PRESSURE_SOLVER2_HPP

#include <Core/Field/ConstantScalarField.hpp>
#include <Core/Field/ConstantVectorField.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Solver/Grid/GridBoundaryConditionSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D grid-based pressure solver.
//!
//! This class represents a 2-D grid-based pressure solver interface which can
//! be used as a sub-step of GridFluidSolver2. Inheriting classes must implement
//! the core GridPressureSolver2::Solve function as well as the helper function
//! GridPressureSolver2::SuggestedBoundaryConditionSolver.
//!
class GridPressureSolver2
{
 public:
    //! Default constructor.
    GridPressureSolver2() = default;

    //! Deleted copy constructor.
    GridPressureSolver2(const GridPressureSolver2&) = delete;

    //! Deleted move constructor.
    GridPressureSolver2(GridPressureSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    virtual ~GridPressureSolver2() = default;

    //! Deleted copy assignment operator.
    GridPressureSolver2& operator=(const GridPressureSolver2&) = delete;

    //! Deleted move assignment operator.
    GridPressureSolver2& operator=(GridPressureSolver2&&) noexcept = delete;

    //!
    //! \brief Solves the pressure term and apply it to the velocity field.
    //!
    //! This function takes input velocity field and outputs pressure-applied
    //! velocity field. It also accepts extra arguments such as \p boundarySDF
    //! and \p fluidSDF that represent signed-distance representation of the
    //! boundary and fluid area. The negative region of \p boundarySDF means
    //! it is occupied by solid object. Also, the positive / negative area of
    //! the \p fluidSDF means it is occupied by fluid / atmosphere. If not
    //! specified, constant scalar field with std::numeric_limits<double>::max()
    //! will be used for \p boundarySDF meaning that no boundary at all.
    //! Similarly, a constant field with -std::numeric_limits<double>::max()
    //! will be used for \p fluidSDF which means it's fully occupied with fluid
    //! without any atmosphere.
    //!
    //! \param[in]     input                 The input velocity field.
    //! \param[in]     timeIntervalInSeconds The time interval for the sim.
    //! \param[in,out] output                The output velocity field.
    //! \param[in]     boundarySDF           The SDF of the boundary.
    //! \param[in]     boundaryVelocity      The velocity of the boundary.
    //! \param[in]     fluidSDF              The SDF of the fluid/atmosphere.
    //! \param[in]     useCompressed         True if it uses compressed system.
    //!
    virtual void Solve(
        const FaceCenteredGrid2& input, double timeIntervalInSeconds,
        FaceCenteredGrid2* output,
        const ScalarField2& boundarySDF =
            ConstantScalarField2{ std::numeric_limits<double>::max() },
        const VectorField2& boundaryVelocity = ConstantVectorField2{ { 0, 0 } },
        const ScalarField2& fluidSDF =
            ConstantScalarField2{ -std::numeric_limits<double>::max() },
        bool useCompressed = false) = 0;

    //!
    //! \brief Returns the best boundary condition solver for this solver.
    //!
    //! This function returns the best boundary condition solver that works well
    //! with this pressure solver. Depending on the pressure solver
    //! implementation, different boundary condition solver might be used.
    //!
    [[nodiscard]] virtual GridBoundaryConditionSolver2Ptr
    SuggestedBoundaryConditionSolver() const = 0;
};

//! Shared pointer type for the GridPressureSolver2.
using GridPressureSolver2Ptr = std::shared_ptr<GridPressureSolver2>;
}  // namespace CubbyFlow

#endif