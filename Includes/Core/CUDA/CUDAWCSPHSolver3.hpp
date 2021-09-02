// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_WC_SPH_SOLVER3_HPP
#define CUBBYFLOW_CUDA_WC_SPH_SOLVER3_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDASPHSolverBase3.hpp>

namespace CubbyFlow
{
//!
//! \brief CUDA-based 3-D WCSPH solver.
//!
//! This class implements 3-D WCSPH solver using CUDA. The main pressure solver
//! is based on equation-of-state (EOS).
//!
//! \see CUDAParticleSystemSolver3
//! \see SPHSolver3
//!
//! \see M{\"u}ller et al., Particle-based fluid simulation for interactive
//!      applications, SCA 2003.
//! \see M. Becker and M. Teschner, Weakly compressible SPH for free surface
//!      flows, SCA 2007.
//! \see Adams and Wicke, Meshless approximation methods and applications in
//!      physics based modeling and animation, Eurographics tutorials 2009.
//!
class CUDAWCSPHSolver3 : public CUDASPHSolverBase3
{
 public:
    class Builder;

    //! Constructs a solver with empty particle set.
    CUDAWCSPHSolver3();

    //! Constructs a solver with target density, spacing, and relative kernel
    //! radius.
    CUDAWCSPHSolver3(float targetDensity, float targetSpacing,
                     float relativeKernelRadius);

    //! Deleted copy constructor.
    CUDAWCSPHSolver3(const CUDAWCSPHSolver3&) = delete;

    //! Deleted move constructor.
    CUDAWCSPHSolver3(CUDAWCSPHSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~CUDAWCSPHSolver3() override = default;

    //! Deleted copy assignment operator.
    CUDAWCSPHSolver3& operator=(const CUDAWCSPHSolver3&) = delete;

    //! Deleted move assignment operator.
    CUDAWCSPHSolver3& operator=(CUDAWCSPHSolver3&&) noexcept = delete;

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

    //! Returns builder fox CUDAWCSPHSolver3.
    static Builder GetBuilder();

 protected:
    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

 private:
    float m_eosExponent = 7.0f;
};

//! Shared pointer type for the CUDAWCSPHSolver3.
using CUDAWCSPHSolver3Ptr = std::shared_ptr<CUDAWCSPHSolver3>;

//!
//! \brief Front-end to create CUDAWCSPHSolver3 objects step by step.
//!
class CUDAWCSPHSolver3::Builder final
    : public CUDASPHSolverBuilderBase3<CUDAWCSPHSolver3::Builder>
{
 public:
    //! Builds CUDAWCSPHSolver3.
    [[nodiscard]] CUDAWCSPHSolver3 Build() const;

    //! Builds shared pointer of CUDAWCSPHSolver3 instance.
    [[nodiscard]] CUDAWCSPHSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif

#endif