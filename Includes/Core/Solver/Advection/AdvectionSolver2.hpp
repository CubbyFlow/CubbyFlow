// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ADVECTION_SOLVER2_HPP
#define CUBBYFLOW_ADVECTION_SOLVER2_HPP

#include <Core/Field/ConstantScalarField.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Grid/CollocatedVectorGrid.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract based class for 2-D grid-based advection solver.
//!
//! The implementation of this abstract base class should solve 2-D advection
//! equation for scalar and vector fields.
//!
class AdvectionSolver2
{
 public:
    //! Default constructor.
    AdvectionSolver2() = default;

    //! Default copy constructor.
    AdvectionSolver2(const AdvectionSolver2&) = default;

    //! Default move constructor.
    AdvectionSolver2(AdvectionSolver2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~AdvectionSolver2() = default;

    //! Default copy assignment operator.
    AdvectionSolver2& operator=(const AdvectionSolver2&) = default;

    //! Default move assignment operator.
    AdvectionSolver2& operator=(AdvectionSolver2&&) noexcept = default;

    //!
    //! \brief Solves advection equation for given scalar grid.
    //!
    //! The implementation of this virtual function should solve advection
    //! equation for given scalar field \p input and underlying vector field
    //! \p flow that carries the input field. The solution after solving the
    //! equation for given time-step \p dt should be stored in scalar field
    //! \p output. The boundary interface is given by a signed-distance field.
    //! The field is negative inside the boundary. By default, a constant field
    //! with max double value (std::numeric_limits<double>::max()) is used,
    //! meaning no boundary.
    //!
    //! \param input Input scalar grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output scalar grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const ScalarGrid2& input, const VectorField2& flow,
                        double dt, ScalarGrid2* output,
                        const ScalarField2& boundarySDF = ConstantScalarField2(
                            std::numeric_limits<double>::max())) = 0;

    //!
    //! \brief Solves advection equation for given collocated vector grid.
    //!
    //! The implementation of this virtual function should solve advection
    //! equation for given collocated vector grid \p input and underlying vector
    //! field \p flow that carries the input field. The solution after solving
    //! the equation for given time-step \p dt should be stored in vector field
    //! \p output. The boundary interface is given by a signed-distance field.
    //! The field is negative inside the boundary. By default, a constant field
    //! with max double value (std::numeric_limits<double>::max()) is used,
    //! meaning no boundary.
    //!
    //! \param input Input vector grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output vector grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const CollocatedVectorGrid2& input,
                        const VectorField2& flow, double dt,
                        CollocatedVectorGrid2* output,
                        const ScalarField2& boundarySDF = ConstantScalarField2(
                            std::numeric_limits<double>::max()));

    //!
    //! \brief Solves advection equation for given face-centered vector grid.
    //!
    //! The implementation of this virtual function should solve advection
    //! equation for given face-centered vector field \p input and underlying
    //! vector field \p flow that carries the input field. The solution after
    //! solving the equation for given time-step \p dt should be stored in
    //! vector field \p output. The boundary interface is given by a
    //! signed-distance field. The field is negative inside the boundary. By
    //! default, a constant field with max double value
    //! (std::numeric_limits<double>::max()) is used, meaning no boundary.
    //!
    //! \param input Input vector grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output vector grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const FaceCenteredGrid2& input,
                        const VectorField2& flow, double dt,
                        FaceCenteredGrid2* output,
                        const ScalarField2& boundarySDF = ConstantScalarField2(
                            std::numeric_limits<double>::max()));
};

//! Shared pointer type for the 2-D advection solver.
using AdvectionSolver2Ptr = std::shared_ptr<AdvectionSolver2>;
}  // namespace CubbyFlow

#endif