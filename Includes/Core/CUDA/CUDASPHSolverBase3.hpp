// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SPH_SOLVER_BASE3_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SPH_SOLVER_BASE3_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAParticleSystemSolverBase3.hpp>
#include <Core/CUDA/CUDASPHSystemData3.hpp>
#include <Core/Geometry/BoundingBox.hpp>

namespace CubbyFlow
{
//!
//! \brief CUDA-based 3-D SPH solver.
//!
class CUDASPHSolverBase3 : public CUDAParticleSystemSolverBase3
{
 public:
    //! Constructs a solver with empty particle set.
    CUDASPHSolverBase3();

    //! Deleted copy constructor.
    CUDASPHSolverBase3(const CUDASPHSolverBase3&) = delete;

    //! Deleted move constructor.
    CUDASPHSolverBase3(CUDASPHSolverBase3&&) noexcept = delete;

    //! Virtual default destructor.
    ~CUDASPHSolverBase3() override = default;

    //! Deleted copy assignment operator.
    CUDASPHSolverBase3& operator=(const CUDASPHSolverBase3&) = delete;

    //! Deleted move assignment operator.
    CUDASPHSolverBase3& operator=(CUDASPHSolverBase3&&) noexcept = delete;

    //!
    //! \brief Negative pressure scaling factor.
    //!
    //! Zero means clamping. One means do nothing.
    //!
    float NegativePressureScale() const;

    //!
    //! \brief Sets the negative pressure scale.
    //!
    //! This function sets the negative pressure scale. By setting the number
    //! between 0 and 1, the solver will scale the effect of negative pressure
    //! which can prevent the clumping of the particles near the surface. Input
    //! value outside 0 and 1 will be clamped within the range. Default is 0.
    //!
    void SetNegativePressureScale(float newNegativePressureScale);

    //! Returns the viscosity coefficient.
    float ViscosityCoefficient() const;

    //! Sets the viscosity coefficient.
    void SetViscosityCoefficient(float newViscosityCoefficient);

    //!
    //! \brief Pseudo-viscosity coefficient velocity filtering.
    //!
    //! This is a minimum "safety-net" for SPH solver which is quite
    //! sensitive to the parameters.
    //!
    float PseudoViscosityCoefficient() const;

    //! Sets the pseudo viscosity coefficient.
    void SetPseudoViscosityCoefficient(float newPseudoViscosityCoefficient);

    //!
    //! \brief Speed of sound in medium to determine the stiffness of the
    //! system.
    //!
    //! Ideally, it should be the actual speed of sound in the fluid, but in
    //! practice, use lower value to trace-off performance and compressibility.
    //!
    float SpeedOfSound() const;

    //! Sets the speed of sound.
    void SetSpeedOfSound(float newSpeedOfSound);

    //!
    //! \brief Multiplier that scales the max allowed time-step.
    //!
    //! This function returns the multiplier that scales the max allowed
    //! time-step. When the scale is 1.0, the time-step is bounded by the speed
    //! of sound and max acceleration.
    //!
    float TimeStepLimitScale() const;

    //! Sets the multiplier that scales the max allowed time-step.
    void SetTimeStepLimitScale(float newScale);

    //!
    //! \brief Returns the container of the simulation.
    //!
    //! This function returns the container which bounds the particles. The
    //! default size of the container is [-1000, -1000] x [1000, 1000].
    //!
    const BoundingBox3F& Container() const;

    //! Sets the container of the simulation.
    void SetContainer(const BoundingBox3F& cont);

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    CUDAParticleSystemData3* ParticleSystemData() override;

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    const CUDAParticleSystemData3* ParticleSystemData() const override;

    //!
    //! \brief Returns the SPH system data.
    //!
    //! This function returns the SPH system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    CUDASPHSystemData3* SPHSystemData();

    //!
    //! \brief Returns the SPH system data.
    //!
    //! This function returns the SPH system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    const CUDASPHSystemData3* SPHSystemData() const;

 protected:
    //! Returns the number of sub-time-steps.
    unsigned int GetNumberOfSubTimeSteps(
        double timeIntervalInSeconds) const override;

    CUDAArrayView1<float4> Forces() const;

    CUDAArrayView1<float4> SmoothedVelocities() const;

 private:
    // Basic SPH solver properties
    size_t m_forcesIdx;
    size_t m_smoothedVelIdx;
    float m_negativePressureScale = 0.0f;
    float m_viscosityCoefficient = 0.01f;
    float m_pseudoViscosityCoefficient = 10.0f;
    float m_speedOfSound = 100.0f;
    float m_timeStepLimitScale = 1.0f;
    BoundingBox3F m_container;

    // Data model
    CUDASPHSystemData3Ptr m_sphSystemData;
};

//! Shared pointer type for the CUDASPHSolverBase3.
using CUDASPHSolverBase3Ptr = std::shared_ptr<CUDASPHSolverBase3>;

//!
//! \brief Base class for CUDA-based SPH solver builder.
//!
template <typename DerivedBuilder>
class CUDASPHSolverBuilderBase3
    : public CUDAParticleSystemSolverBuilderBase3<DerivedBuilder>
{
 public:
    //! Returns builder with target density.
    DerivedBuilder& WithTargetDensity(float targetDensity);

    //! Returns builder with target spacing.
    DerivedBuilder& WithTargetSpacing(float targetSpacing);

    //! Returns builder with relative kernel radius.
    DerivedBuilder& WithRelativeKernelRadius(float relativeKernelRadius);

    //! Returns builder with negative pressure scale.
    DerivedBuilder& WithNegativePressureScale(float negativePressureScale);

    //! Returns builder with viscosity coefficient.
    DerivedBuilder& WithViscosityCoefficient(float viscosityCoefficient);

    //! Returns builder with pseudo viscosity coefficient.
    DerivedBuilder& WithPseudoViscosityCoefficient(
        float pseudoViscosityCoefficient);

 protected:
    float m_targetDensity = WATER_DENSITY_FLOAT;
    float m_targetSpacing = 0.1f;
    float m_relativeKernelRadius = 1.8f;
    float m_negativePressureScale = 0.0f;
    float m_viscosityCoefficient = 0.01f;
    float m_pseudoViscosityCoefficient = 10.0f;
};

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithTargetDensity(float targetDensity)
{
    m_targetDensity = targetDensity;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithTargetSpacing(float targetSpacing)
{
    m_targetSpacing = targetSpacing;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithRelativeKernelRadius(
    float relativeKernelRadius)
{
    m_relativeKernelRadius = relativeKernelRadius;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithNegativePressureScale(
    float negativePressureScale)
{
    m_negativePressureScale = negativePressureScale;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithViscosityCoefficient(
    float viscosityCoefficient)
{
    m_viscosityCoefficient = viscosityCoefficient;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDASPHSolverBuilderBase3<T>::WithPseudoViscosityCoefficient(
    float pseudoViscosityCoefficient)
{
    m_pseudoViscosityCoefficient = pseudoViscosityCoefficient;
    return static_cast<T&>(*this);
}
}  // namespace CubbyFlow

#endif

#endif