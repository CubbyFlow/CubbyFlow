// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FLIP_SOLVER2_HPP
#define CUBBYFLOW_FLIP_SOLVER2_HPP

#include <Core/Solver/Hybrid/PIC/PICSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D Fluid-Implicit Particle (FLIP) implementation.
//!
//! This class implements 2-D Fluid-Implicit Particle (FLIP) solver from the
//! SIGGRAPH paper, Zhu and Bridson 2005. By transferring delta-velocity field
//! from grid to particles, the FLIP solver achieves less viscous fluid flow
//! compared to the original PIC method.
//!
//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
//!     ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
//!
class FLIPSolver2 : public PICSolver2
{
 public:
    class Builder;

    //! Default constructor.
    FLIPSolver2();

    //! Constructs solver with initial grid size.
    FLIPSolver2(const Vector2UZ& resolution, const Vector2D& gridSpacing,
                const Vector2D& gridOrigin);

    //! Deleted copy constructor.
    FLIPSolver2(const FLIPSolver2&) = delete;

    //! Deleted move constructor.
    FLIPSolver2(FLIPSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~FLIPSolver2() override = default;

    //! Deleted copy assignment operator.
    FLIPSolver2& operator=(const FLIPSolver2&) = delete;

    //! Deleted move assignment operator.
    FLIPSolver2& operator=(FLIPSolver2&&) noexcept = delete;

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

    //! Returns builder fox FLIPSolver2.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Transfers velocity field from particles to grids.
    void TransferFromParticlesToGrids() override;

    //! Transfers velocity field from grids to particles.
    void TransferFromGridsToParticles() override;

 private:
    double m_picBlendingFactor = 0.0;
    Array2<double> m_uDelta;
    Array2<double> m_vDelta;
};

//! Shared pointer type for the FLIPSolver2.
using FLIPSolver2Ptr = std::shared_ptr<FLIPSolver2>;

//!
//! \brief Front-end to create FLIPSolver2 objects step by step.
//!
class FLIPSolver2::Builder final : public GridFluidSolverBuilderBase2<Builder>
{
 public:
    //! Builds FLIPSolver2.
    [[nodiscard]] FLIPSolver2 Build() const;

    //! Builds shared pointer of FLIPSolver2 instance.
    [[nodiscard]] FLIPSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif