// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER_BASE3_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER_BASE3_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/Animation/PhysicsAnimation.hpp>
#include <Core/CUDA/CUDAParticleSystemData3.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for CUDA-based 3-D particle solvers.
//!
//! This class defines commonly used particle-related interface for CUDA-based
//! particle solvers.
//!
class CUDAParticleSystemSolverBase3 : public PhysicsAnimation
{
 public:
    //! Constructs an empty solver.
    CUDAParticleSystemSolverBase3();

    //! Deleted copy constructor.
    CUDAParticleSystemSolverBase3(const CUDAParticleSystemSolverBase3&) =
        delete;

    //! Deleted move constructor.
    CUDAParticleSystemSolverBase3(CUDAParticleSystemSolverBase3&&) noexcept =
        delete;

    //! Virtual default destructor.
    ~CUDAParticleSystemSolverBase3() override = default;

    //! Deleted copy assignment operator.
    CUDAParticleSystemSolverBase3& operator=(
        const CUDAParticleSystemSolverBase3&) = delete;

    //! Deleted move assignment operator.
    CUDAParticleSystemSolverBase3& operator=(
        CUDAParticleSystemSolverBase3&&) noexcept = delete;

    //! The amount of air-drag.
    float DragCoefficient() const;

    //!
    //! \brief Sets the drag coefficient.
    //!
    //! The coefficient should be a positive number and 0 means no drag force.
    //!
    //! \param newDragCoefficient The new drag coefficient.
    //!
    void SetDragCoefficient(float newDragCoefficient);

    //!
    //! \brief The restitution coefficient.
    //!
    //! The restitution coefficient controls the bouncy-ness of a particle when
    //! it hits a collider surface. 0 means no bounce back and 1 means perfect
    //! reflection.
    //!
    float RestitutionCoefficient() const;

    //!
    //! \brief Sets the restitution coefficient.
    //!
    //! The range of the coefficient should be 0 to 1 -- 0 means no bounce back
    //! and 1 means perfect reflection.
    //!
    //! \param newRestitutionCoefficient The new restitution coefficient.
    //!
    void SetRestitutionCoefficient(float newRestitutionCoefficient);

    //! Returns the gravity.
    const Vector3F& Gravity() const;

    //! Sets the gravity.
    void SetGravity(const Vector3F& newGravity);

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    virtual CUDAParticleSystemData3* ParticleSystemData();

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    virtual const CUDAParticleSystemData3* ParticleSystemData() const;

 protected:
    //! Initializes the simulator.
    void OnInitialize() override;

    void UpdateCollider(double timeStepInSeconds);

    void UpdateEmitter(double timeStepInSeconds);

 private:
    float m_dragCoefficient = 1e-4f;
    float m_restitutionCoefficient = 0.0f;
    Vector3F m_gravity{ 0.0f, GRAVITY_FLOAT, 0.0f };
    CUDAParticleSystemData3Ptr m_particleSystemData;
};

//! Shared pointer type for the CUDAParticleSystemSolverBase3.
using CUDAParticleSystemSolverBase3Ptr =
    std::shared_ptr<CUDAParticleSystemSolverBase3>;

//!
//! \brief Base class for CUDA-based particle solver builder.
//!
template <typename DerivedBuilder>
class CUDAParticleSystemSolverBuilderBase3
{
 public:
    //! Returns builder with particle drag coefficient.
    DerivedBuilder& WithDragCoefficient(float coeff);

    //! Returns builder with particle restitution coefficient.
    DerivedBuilder& WithRestitutionCoefficient(float coeff);

    //! Returns builder with particle gravity.
    DerivedBuilder& WithGravity(const Vector3F& gravity);

 protected:
    float m_dragCoefficient = 1e-4f;
    float m_restitutionCoefficient = 0.0f;
    Vector3F m_gravity{ 0.0f, GRAVITY_FLOAT, 0.0f };
};

template <typename T>
T& CUDAParticleSystemSolverBuilderBase3<T>::WithDragCoefficient(float coeff)
{
    m_dragCoefficient = coeff;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDAParticleSystemSolverBuilderBase3<T>::WithRestitutionCoefficient(
    float coeff)
{
    m_restitutionCoefficient = coeff;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDAParticleSystemSolverBuilderBase3<T>::WithGravity(const Vector3F& gravity)
{
    m_gravity = gravity;
    return static_cast<T&>(*this);
}
}  // namespace CubbyFlow

#endif

#endif