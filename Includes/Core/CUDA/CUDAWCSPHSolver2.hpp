// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_WC_SPH_SOLVER2_HPP
#define CUBBYFLOW_CUDA_WC_SPH_SOLVER2_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDASPHSolverBase2.hpp>

namespace CubbyFlow
{
//!
//! \brief CUDA-based 2-D WCSPH solver.
//!
//! This class implements 2-D WCSPH solver using CUDA. The main pressure solver
//! is based on equation-of-state (EOS).
//!
//! \see CUDAParticleSystemSolver2
//! \see SPHSolver2
//!
//! \see M{\"u}ller et al., Particle-based fluid simulation for interactive
//!      applications, SCA 2003.
//! \see M. Becker and M. Teschner, Weakly compressible SPH for free surface
//!      flows, SCA 2007.
//! \see Adams and Wicke, Meshless approximation methods and applications in
//!      physics based modeling and animation, Eurographics tutorials 2009.
//!
class CUDAWCSPHSolver2 : public CUDASPHSolverBase2
{
 public:
    class Builder;

    //! Constructs a solver with empty particle set.
    CUDAWCSPHSolver2();

    //! Constructs a solver with target density, spacing, and relative kernel
    //! radius.
    CUDAWCSPHSolver2(float targetDensity, float targetSpacing,
                     float relativeKernelRadius);

    //! Deleted copy constructor.
    CUDAWCSPHSolver2(const CUDAWCSPHSolver2&) = delete;

    //! Deleted move constructor.
    CUDAWCSPHSolver2(CUDAWCSPHSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~CUDAWCSPHSolver2() override = default;

    //! Deleted copy assignment operator.
    CUDAWCSPHSolver2& operator=(const CUDAWCSPHSolver2&) = delete;

    //! Deleted move assignment operator.
    CUDAWCSPHSolver2& operator=(CUDAWCSPHSolver2&&) noexcept = delete;

    //! Exponent component of equation-of-state (or Tait's equation).
    [[nodiscard]] float EOSExponent() const;

    //!
    //! \brief Sets the exponent part of the equation-of-state.
    //!
    //! This function sets the exponent part of the equation-of-state.
    //! The value must be greater than 1.0, and smaller inputs will be clamped.
    //! Default is 7.
    //!
    void SetEOSExponent(float newEosExponent);

    //! Returns builder fox CUDAWCSPHSolver2.
    static Builder GetBuilder();

 protected:
    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

 private:
    float m_eosExponent = 7.0f;
};

//! Shared pointer type for the CUDAWCSPHSolver2.
using CUDAWCSPHSolver2Ptr = std::shared_ptr<CUDAWCSPHSolver2>;

//!
//! \brief Front-end to create CUDAWCSPHSolver2 objects step by step.
//!
class CUDAWCSPHSolver2::Builder final
    : public CUDASPHSolverBuilderBase2<CUDAWCSPHSolver2::Builder>
{
 public:
    //! Builds CUDAWCSPHSolver2.
    [[nodiscard]] CUDAWCSPHSolver2 Build() const;

    //! Builds shared pointer of CUDAWCSPHSolver2 instance.
    [[nodiscard]] CUDAWCSPHSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif

#endif