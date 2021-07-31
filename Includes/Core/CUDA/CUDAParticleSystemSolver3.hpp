// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER3_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER3_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAParticleSystemSolverBase3.hpp>

namespace CubbyFlow
{
//!
//! \brief CUDA-based basic 3-D particle system solver.
//!
//! This class implements basic particle system solver with CUDA. It includes
//! gravity, air drag, and collision. But it does not compute
//! particle-to-particle interaction. Thus, this solver is suitable for
//! performing simple spray-like simulations with low computational cost. This
//! class can be further extend to add more sophisticated simulations, such as
//! SPH, to handle particle-to-particle intersection.
//!
//! \see ParticleSystemSolver3
//! \see SPHSolver3
//!
class CUDAParticleSystemSolver3 : public CUDAParticleSystemSolverBase3
{
 public:
    class Builder;

    //! Constructs an empty solver.
    CUDAParticleSystemSolver3();

    //! Constructs a solver with particle parameters.
    CUDAParticleSystemSolver3(float radius, float mass);

    //! Deleted copy constructor.
    CUDAParticleSystemSolver3(const CUDAParticleSystemSolver3&) = delete;

    //! Deleted move constructor.
    CUDAParticleSystemSolver3(CUDAParticleSystemSolver3&&) noexcept = delete;

    //! Virtual default destructor.
    ~CUDAParticleSystemSolver3() override = default;

    //! Deleted copy assignment operator.
    CUDAParticleSystemSolver3& operator=(const CUDAParticleSystemSolver3&) =
        delete;

    //! Deleted move assignment operator.
    CUDAParticleSystemSolver3& operator=(CUDAParticleSystemSolver3&&) noexcept =
        delete;

    //! Radius of a particle.
    float Radius() const;

    //! Sets the radius of a particle.
    void SetRadius(float newRadius);

    //! Mass of a particle.
    float Mass() const;

    //! Sets the mass of a particle.
    void SetMass(float newMass);

    //! Returns builder fox CUDAParticleSystemSolver3.
    static Builder GetBuilder();

 protected:
    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

 private:
    float m_radius = 1e-3f;
    float m_mass = 1e-3f;
};

//! Shared pointer type for the CUDAParticleSystemSolver3.
using CUDAParticleSystemSolver3Ptr = std::shared_ptr<CUDAParticleSystemSolver3>;

//!
//! \brief Front-end to create CUDAParticleSystemSolver3 objects step by step.
//!
class CUDAParticleSystemSolver3::Builder final
    : public CUDAParticleSystemSolverBuilderBase3<Builder>
{
 public:
    //! Returns builder with particle radius.
    Builder& WithRadius(float radius);

    //! Returns builder with mass per particle.
    Builder& WithMass(float mass);

    //! Builds CUDAParticleSystemSolver3.
    CUDAParticleSystemSolver3 Build() const;

    //! Builds shared pointer of CUDAParticleSystemSolver3 instance.
    CUDAParticleSystemSolver3Ptr MakeShared() const;

 private:
    float m_radius = 1e-3f;
    float m_mass = 1e-3f;
};
}  // namespace CubbyFlow

#endif

#endif