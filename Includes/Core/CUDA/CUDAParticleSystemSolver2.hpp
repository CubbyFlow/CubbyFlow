// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER2_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER2_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAParticleSystemSolverBase2.hpp>

namespace CubbyFlow
{
//!
//! \brief CUDA-based basic 2-D particle system solver.
//!
//! This class implements basic particle system solver with CUDA. It includes
//! gravity, air drag, and collision. But it does not compute
//! particle-to-particle interaction. Thus, this solver is suitable for
//! performing simple spray-like simulations with low computational cost. This
//! class can be further extend to add more sophisticated simulations, such as
//! SPH, to handle particle-to-particle intersection.
//!
//! \see ParticleSystemSolver2
//! \see SPHSolver2
//!
class CUDAParticleSystemSolver2 : public CUDAParticleSystemSolverBase2
{
 public:
    class Builder;

    //! Constructs an empty solver.
    CUDAParticleSystemSolver2();

    //! Constructs a solver with particle parameters.
    CUDAParticleSystemSolver2(float radius, float mass);

    //! Deleted copy constructor.
    CUDAParticleSystemSolver2(const CUDAParticleSystemSolver2&) = delete;

    //! Deleted move constructor.
    CUDAParticleSystemSolver2(CUDAParticleSystemSolver2&&) noexcept = delete;

    //! Virtual default destructor.
    ~CUDAParticleSystemSolver2() override = default;

    //! Deleted copy assignment operator.
    CUDAParticleSystemSolver2& operator=(const CUDAParticleSystemSolver2&) =
        delete;

    //! Deleted move assignment operator.
    CUDAParticleSystemSolver2& operator=(CUDAParticleSystemSolver2&&) noexcept =
        delete;

    //! Radius of a particle.
    float Radius() const;

    //! Sets the radius of a particle.
    void SetRadius(float newRadius);

    //! Mass of a particle.
    float Mass() const;

    //! Sets the mass of a particle.
    void SetMass(float newMass);

    //! Returns builder fox CUDAParticleSystemSolver2.
    static Builder GetBuilder();

 protected:
    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

 private:
    float m_radius = 1e-3f;
    float m_mass = 1e-3f;
};

//! Shared pointer type for the CUDAParticleSystemSolver2.
using CUDAParticleSystemSolver2Ptr = std::shared_ptr<CUDAParticleSystemSolver2>;

//!
//! \brief Front-end to create CUDAParticleSystemSolver2 objects step by step.
//!
class CUDAParticleSystemSolver2::Builder final
    : public CUDAParticleSystemSolverBuilderBase2<Builder>
{
 public:
    //! Returns builder with particle radius.
    Builder& WithRadius(float radius);

    //! Returns builder with mass per particle.
    Builder& WithMass(float mass);

    //! Builds CUDAParticleSystemSolver2.
    CUDAParticleSystemSolver2 Build() const;

    //! Builds shared pointer of CUDAParticleSystemSolver2 instance.
    CUDAParticleSystemSolver2Ptr MakeShared() const;

 private:
    float m_radius = 1e-3f;
    float m_mass = 1e-3f;
};
}  // namespace CubbyFlow

#endif

#endif