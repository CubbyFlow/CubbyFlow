// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ADVECTION_SOLVER3_HPP
#define CUBBYFLOW_ADVECTION_SOLVER3_HPP

#include <Core/Field/ConstantScalarField.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Grid/CollocatedVectorGrid.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract based class for 3-D grid-based advection solver.
//!
//! The implementation of this abstract base class should solve 3-D advection
//! equation for scalar and vector fields.
//!
class AdvectionSolver3
{
 public:
    //! Default constructor.
    AdvectionSolver3() = default;

    //! Default copy constructor.
    AdvectionSolver3(const AdvectionSolver3&) = default;

    //! Default move constructor.
    AdvectionSolver3(AdvectionSolver3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~AdvectionSolver3() = default;

    //! Default copy assignment operator.
    AdvectionSolver3& operator=(const AdvectionSolver3&) = default;

    //! Default move assignment operator.
    AdvectionSolver3& operator=(AdvectionSolver3&&) noexcept = default;

    //!
    //! \brief Solves advection equation for given scalar grid.
    //!
    //! The implementation of this virtual function should solve advection
    //! equation for given scalar field \p input and underlying vector field
    //! \p flow that carries the input field. The solution after solving the
    //! equation for given time-step \p dt should be stored in scalar field
    //! \p output. The boundary interface is given by a signed-distance field.
    //! The field is negative inside the boundary. By default, a constant field
    //! with max double value (std::numeric_limists<double>::max())
    //! is used, meaning no boundary.
    //!
    //! \param input Input scalar grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output scalar grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const ScalarGrid3& input, const VectorField3& flow,
                        double dt, ScalarGrid3* output,
                        const ScalarField3& boundarySDF = ConstantScalarField3(
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
    //! with max double value (std::numeric_limists<double>::max())
    //! is used, meaning no boundary.
    //!
    //! \param input Input vector grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output vector grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const CollocatedVectorGrid3& input,
                        const VectorField3& flow, double dt,
                        CollocatedVectorGrid3* output,
                        const ScalarField3& boundarySDF = ConstantScalarField3(
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
    //! (std::numeric_limists<double>::max()) is used, meaning no boundary.
    //!
    //! \param input Input vector grid.
    //! \param flow Vector field that advects the input field.
    //! \param dt Time-step for the advection.
    //! \param output Output vector grid.
    //! \param boundarySDF Boundary interface defined by signed-distance
    //!     field.
    //!
    virtual void Advect(const FaceCenteredGrid3& input,
                        const VectorField3& flow, double dt,
                        FaceCenteredGrid3* output,
                        const ScalarField3& boundarySDF = ConstantScalarField3(
                            std::numeric_limits<double>::max()));
};

//! Shared pointer type for the 3-D advection solver.
using AdvectionSolver3Ptr = std::shared_ptr<AdvectionSolver3>;
}  // namespace CubbyFlow

#endif