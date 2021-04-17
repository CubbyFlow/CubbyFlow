// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_DIFFUSION_SOLVER2_HPP
#define CUBBYFLOW_GRID_DIFFUSION_SOLVER2_HPP

#include <Core/Field/ConstantScalarField.hpp>
#include <Core/Grid/CollocatedVectorGrid.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D grid-based diffusion equation solver.
//!
//! This class provides functions to solve the diffusion equation for different
//! types of fields. The target equation can be written as
//! \f$\frac{\partial f}{\partial t} = \mu\nabla^2 f\f$ where \f$\mu\f$ is
//! the diffusion coefficient. The field \f$f\f$ can be either scalar or vector
//! field.
//!
class GridDiffusionSolver2
{
 public:
    //! Default constructor.
    GridDiffusionSolver2() = default;

    //! Deleted copy constructor.
    GridDiffusionSolver2(const GridDiffusionSolver2&) = delete;

    //! Deleted move constructor.
    GridDiffusionSolver2(GridDiffusionSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    virtual ~GridDiffusionSolver2() = default;

    //! Deleted copy assignment operator.
    GridDiffusionSolver2& operator=(const GridDiffusionSolver2&) = delete;

    //! Deleted move assignment operator.
    GridDiffusionSolver2& operator=(GridDiffusionSolver2&&) noexcept = delete;

    //!
    //! Solves diffusion equation for a scalar field.
    //!
    //! \param source Input scalar field.
    //! \param diffusionCoefficient Amount of diffusion.
    //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
    //! \param dest Output scalar field.
    //! \param boundarySDF Shape of the solid boundary that is empty by default.
    //! \param fluidSDF Shape of the fluid boundary that is full by default.
    //!
    virtual void Solve(const ScalarGrid2& source, double diffusionCoefficient,
                       double timeIntervalInSeconds, ScalarGrid2* dest,
                       const ScalarField2& boundarySDF =
                           ConstantScalarField2{
                               std::numeric_limits<double>::max() },
                       const ScalarField2& fluidSDF = ConstantScalarField2{
                           -std::numeric_limits<double>::max() }) = 0;

    //!
    //! Solves diffusion equation for a collocated vector field.
    //!
    //! \param source Input collocated vector field.
    //! \param diffusionCoefficient Amount of diffusion.
    //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
    //! \param dest Output collocated vector field.
    //! \param boundarySDF Shape of the solid boundary that is empty by default.
    //! \param fluidSDF Shape of the fluid boundary that is full by default.
    //!
    virtual void Solve(
        const CollocatedVectorGrid2& source, double diffusionCoefficient,
        double timeIntervalInSeconds, CollocatedVectorGrid2* dest,
        const ScalarField2& boundarySDF =
            ConstantScalarField2{ std::numeric_limits<double>::max() },
        const ScalarField2& fluidSDF = ConstantScalarField2{
            -std::numeric_limits<double>::max() }) = 0;

    //!
    //! Solves diffusion equation for a face-centered vector field.
    //!
    //! \param source Input face-centered vector field.
    //! \param diffusionCoefficient Amount of diffusion.
    //! \param timeIntervalInSeconds Small time-interval that diffusion occur.
    //! \param dest Output face-centered vector field.
    //! \param boundarySDF Shape of the solid boundary that is empty by default.
    //! \param fluidSDF Shape of the fluid boundary that is full by default.
    //!
    virtual void Solve(const FaceCenteredGrid2& source,
                       double diffusionCoefficient,
                       double timeIntervalInSeconds, FaceCenteredGrid2* dest,
                       const ScalarField2& boundarySDF =
                           ConstantScalarField2{
                               std::numeric_limits<double>::max() },
                       const ScalarField2& fluidSDF = ConstantScalarField2{
                           -std::numeric_limits<double>::max() }) = 0;
};

//! Shared pointer type for the GridDiffusionSolver2.
using GridDiffusionSolver2Ptr = std::shared_ptr<GridDiffusionSolver2>;
}  // namespace CubbyFlow

#endif