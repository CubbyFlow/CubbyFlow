// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SEMI_LAGRANGIAN3_HPP
#define CUBBYFLOW_SEMI_LAGRANGIAN3_HPP

#include <Core/Solver/Advection/AdvectionSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief Implementation of 3-D semi-Lagrangian advection solver.
//!
//! This class implements 3-D semi-Lagrangian advection solver. By default, the
//! class implements 1st-order (linear) algorithm for the spatial interpolation.
//! For the back-tracing, this class uses 2nd-order mid-point rule with adaptive
//! time-stepping (CFL <= 1).
//! To extend the class using higher-order spatial interpolation, the inheriting
//! classes can override SemiLagrangian2::getScalarSamplerFunc and
//! SemiLagrangian2::getVectorSamplerFunc. See CubicSemiLagrangian2 for example.
//!
class SemiLagrangian3 : public AdvectionSolver3
{
 public:
    //! Default constructor.
    SemiLagrangian3() = default;

    //! Default copy constructor.
    SemiLagrangian3(const SemiLagrangian3&) = default;

    //! Default move constructor.
    SemiLagrangian3(SemiLagrangian3&&) noexcept = default;

    //! Virtual default destructor.
    ~SemiLagrangian3() override = default;

    //! Default copy assignment operator.
    SemiLagrangian3& operator=(const SemiLagrangian3&) = default;

    //! Default move assignment operator.
    SemiLagrangian3& operator=(SemiLagrangian3&&) noexcept = default;

    //!
    //! \brief Computes semi-Lagrangian for given scalar grid.
    //!
    //! This function computes semi-Lagrangian method to solve advection
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
    void Advect(const ScalarGrid3& input, const VectorField3& flow, double dt,
                ScalarGrid3* output,
                const ScalarField3& boundarySDF = ConstantScalarField3(
                    std::numeric_limits<double>::max())) final;

    //!
    //! \brief Computes semi-Lagrangian for given collocated vector grid.
    //!
    //! This function computes semi-Lagrangian method to solve advection
    //! equation for given collocated vector grid \p input and underlying vector
    //! field \p flow that carries the input field. The solution after solving
    //! the equation for given time-step \p dt should be stored in scalar field
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
    void Advect(const CollocatedVectorGrid3& input, const VectorField3& flow,
                double dt, CollocatedVectorGrid3* output,
                const ScalarField3& boundarySDF = ConstantScalarField3(
                    std::numeric_limits<double>::max())) final;

    //!
    //! \brief Computes semi-Lagrangian for given face-centered vector grid.
    //!
    //! This function computes semi-Lagrangian method to solve advection
    //! equation for given face-centered vector grid \p input and underlying
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
    void Advect(const FaceCenteredGrid3& input, const VectorField3& flow,
                double dt, FaceCenteredGrid3* output,
                const ScalarField3& boundarySDF = ConstantScalarField3(
                    std::numeric_limits<double>::max())) final;

 protected:
    //!
    //! \brief Returns spatial interpolation function object for given scalar
    //! grid.
    //!
    //! This function returns spatial interpolation function (sampler) for given
    //! scalar grid \p input. By default, this function returns linear
    //! interpolation function. Override this function to have custom
    //! interpolation for semi-Lagrangian process.
    //!
    [[nodiscard]] virtual std::function<double(const Vector3D&)>
    GetScalarSamplerFunc(const ScalarGrid3& input) const;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! collocated vector grid.
    //!
    //! This function returns spatial interpolation function (sampler) for given
    //! collocated vector grid \p input. By default, this function returns
    //! linear interpolation function. Override this function to have custom
    //! interpolation for semi-Lagrangian process.
    //!
    [[nodiscard]] virtual std::function<Vector3D(const Vector3D&)>
    GetVectorSamplerFunc(const CollocatedVectorGrid3& input) const;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! face-centered vector grid.
    //!
    //! This function returns spatial interpolation function (sampler) for given
    //! face-centered vector grid \p input. By default, this function returns
    //! linear interpolation function. Override this function to have custom
    //! interpolation for semi-Lagrangian process.
    //!
    [[nodiscard]] virtual std::function<Vector3D(const Vector3D&)>
    GetVectorSamplerFunc(const FaceCenteredGrid3& input) const;

 private:
    [[nodiscard]] Vector3D BackTrace(const VectorField3& flow, double dt,
                                     double h, const Vector3D& startPt,
                                     const ScalarField3& boundarySDF) const;
};

using SemiLagrangian3Ptr = std::shared_ptr<SemiLagrangian3>;
}  // namespace CubbyFlow

#endif