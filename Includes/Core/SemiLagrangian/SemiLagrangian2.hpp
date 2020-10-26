// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SEMI_LAGRANGIAN2_HPP
#define CUBBYFLOW_SEMI_LAGRANGIAN2_HPP

#include <Core/Solver/Advection/AdvectionSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief Implementation of 2-D semi-Lagrangian advection solver.
//!
//! This class implements 2-D semi-Lagrangian advection solver. By default, the
//! class implements 1st-order (linear) algorithm for the spatial interpolation.
//! For the back-tracing, this class uses 2nd-order mid-point rule with adaptive
//! time-stepping (CFL <= 1).
//! To extend the class using higher-order spatial interpolation, the inheriting
//! classes can override SemiLagrangian2::getScalarSamplerFunc and
//! SemiLagrangian2::getVectorSamplerFunc. See CubicSemiLagrangian2 for example.
//!
class SemiLagrangian2 : public AdvectionSolver2
{
 public:
    //! Default constructor.
    SemiLagrangian2() = default;

    //! Default copy constructor.
    SemiLagrangian2(const SemiLagrangian2&) = default;

    //! Default move constructor.
    SemiLagrangian2(SemiLagrangian2&&) noexcept = default;

    //! Virtual default destructor.
    ~SemiLagrangian2() override = default;

    //! Default copy assignment operator.
    SemiLagrangian2& operator=(const SemiLagrangian2&) = default;

    //! Default move assignment operator.
    SemiLagrangian2& operator=(SemiLagrangian2&&) noexcept = default;

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
    void Advect(const ScalarGrid2& input, const VectorField2& flow, double dt,
                ScalarGrid2* output,
                const ScalarField2& boundarySDF = ConstantScalarField2(
                    std::numeric_limits<double>::max())) final;

    //!
    //! \brief Computes semi-Lagrangian for given collocated vector grid.
    //!
    //! This function computes semi-Lagrangian method to solve advection
    //! equation for given collocated vector grid \p input and underlying vector
    //! field \p flow that carries the input field. The solution after solving
    //! the equation for given time-step \p dt should be stored in scala1r field
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
    void Advect(const CollocatedVectorGrid2& input, const VectorField2& flow,
                double dt, CollocatedVectorGrid2* output,
                const ScalarField2& boundarySDF = ConstantScalarField2(
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
    void Advect(const FaceCenteredGrid2& input, const VectorField2& flow,
                double dt, FaceCenteredGrid2* output,
                const ScalarField2& boundarySDF = ConstantScalarField2(
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
    [[nodiscard]] virtual std::function<double(const Vector2D&)>
    GetScalarSamplerFunc(const ScalarGrid2& input) const;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! collocated vector grid.
    //!
    //! This function returns spatial interpolation function (sampler) for given
    //! collocated vector grid \p input. By default, this function returns
    //! linear interpolation function. Override this function to have custom
    //! interpolation for semi-Lagrangian process.
    //!
    [[nodiscard]] virtual std::function<Vector2D(const Vector2D&)>
    GetVectorSamplerFunc(const CollocatedVectorGrid2& input) const;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! face-centered vector grid.
    //!
    //! This function returns spatial interpolation function (sampler) for given
    //! face-centered vector grid \p input. By default, this function returns
    //! linear interpolation function. Override this function to have custom
    //! interpolation for semi-Lagrangian process.
    //!
    [[nodiscard]] virtual std::function<Vector2D(const Vector2D&)>
    GetVectorSamplerFunc(const FaceCenteredGrid2& input) const;

 private:
    [[nodiscard]] Vector2D BackTrace(const VectorField2& flow, double dt,
                                     double h, const Vector2D& startPt,
                                     const ScalarField2& boundarySDF) const;
};

using SemiLagrangian2Ptr = std::shared_ptr<SemiLagrangian2>;
}  // namespace CubbyFlow

#endif