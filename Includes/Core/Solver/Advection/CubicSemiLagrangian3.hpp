// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN3_HPP
#define CUBBYFLOW_CUBIC_SEMI_LAGRANGIAN3_HPP

#include <Core/Solver/Advection/SemiLagrangian3.hpp>

namespace CubbyFlow
{
//!
//! \brief Implementation of 3-D cubic semi-Lagrangian advection solver.
//!
//! This class implements 3rd-order cubic 3-D semi-Lagrangian advection solver.
//!
class CubicSemiLagrangian3 final : public SemiLagrangian3
{
 protected:
    //!
    //! \brief Returns spatial interpolation function object for given scalar
    //! grid.
    //!
    //! This function overrides the original function with cubic interpolation.
    //!
    [[nodiscard]] std::function<double(const Vector3D&)> GetScalarSamplerFunc(
        const ScalarGrid3& source) const override;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! collocated vector grid.
    //!
    //! This function overrides the original function with cubic interpolation.
    //!
    [[nodiscard]] std::function<Vector3D(const Vector3D&)> GetVectorSamplerFunc(
        const CollocatedVectorGrid3& source) const override;

    //!
    //! \brief Returns spatial interpolation function object for given
    //! face-centered vector grid.
    //!
    //! This function overrides the original function with cubic interpolation.
    //!
    [[nodiscard]] std::function<Vector3D(const Vector3D&)> GetVectorSamplerFunc(
        const FaceCenteredGrid3& source) const override;
};

using CubicSemiLagrangian3Ptr = std::shared_ptr<CubicSemiLagrangian3>;
}  // namespace CubbyFlow

#endif