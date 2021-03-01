// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FLIP_SOLVER3_HPP
#define CUBBYFLOW_FLIP_SOLVER3_HPP

#include <Core/Solver/Hybrid/PIC/PICSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D Fluid-Implicit Particle (FLIP) implementation.
//!
//! This class implements 3-D Fluid-Implicit Particle (FLIP) solver from the
//! SIGGRAPH paper, Zhu and Bridson 2005. By transferring delta-velocity field
//! from grid to particles, the FLIP solver achieves less viscous fluid flow
//! compared to the original PIC method.
//!
//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
//!     ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
//!
class FLIPSolver3 : public PICSolver3
{
 public:
    class Builder;

    //! Default constructor.
    FLIPSolver3();

    //! Constructs solver with initial grid size.
    FLIPSolver3(const Vector3UZ& resolution, const Vector3D& gridSpacing,
                const Vector3D& gridOrigin);

    //! Deleted copy constructor.
    FLIPSolver3(const FLIPSolver3&) = delete;

    //! Deleted move constructor.
    FLIPSolver3(FLIPSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~FLIPSolver3() override = default;

    //! Deleted copy assignment operator.
    FLIPSolver3& operator=(const FLIPSolver3&) = delete;

    //! Deleted move assignment operator.
    FLIPSolver3& operator=(FLIPSolver3&&) noexcept = delete;

    //! Returns the PIC blending factor.
    [[nodiscard]] double GetPICBlendingFactor() const;

    //!
    //! \brief  Sets the PIC blending factor.
    //!
    //! This function sets the PIC blending factor which mixes FLIP and PIC
    //! results when transferring velocity from grids to particles in order to
    //! reduce the noise. The factor can be a value between 0 and 1, where 0
    //! means no blending and 1 means full PIC. Default is 0.
    //!
    //! \param[in]  factor The blending factor.
    //!
    void SetPICBlendingFactor(double factor);

    //! Returns builder fox FLIPSolver3.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Transfers velocity field from particles to grids.
    void TransferFromParticlesToGrids() override;

    //! Transfers velocity field from grids to particles.
    void TransferFromGridsToParticles() override;

 private:
    double m_picBlendingFactor = 0.0;
    Array3<double> m_uDelta;
    Array3<double> m_vDelta;
    Array3<double> m_wDelta;
};

//! Shared pointer type for the FLIPSolver3.
using FLIPSolver3Ptr = std::shared_ptr<FLIPSolver3>;

//!
//! \brief Front-end to create FLIPSolver3 objects step by step.
//!
class FLIPSolver3::Builder final : public GridFluidSolverBuilderBase3<Builder>
{
 public:
    //! Builds FLIPSolver3.
    [[nodiscard]] FLIPSolver3 Build() const;

    //! Builds shared pointer of FLIPSolver3 instance.
    [[nodiscard]] FLIPSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif