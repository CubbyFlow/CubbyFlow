// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER_BASE2_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SYSTEM_SOLVER_BASE2_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/Animation/PhysicsAnimation.hpp>
#include <Core/CUDA/CUDAParticleSystemData2.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for CUDA-based 2-D particle solvers.
//!
//! This class defines commonly used particle-related interface for CUDA-based
//! particle solvers.
//!
class CUDAParticleSystemSolverBase2 : public PhysicsAnimation
{
 public:
    //! Constructs an empty solver.
    CUDAParticleSystemSolverBase2();

    //! Deleted copy constructor.
    CUDAParticleSystemSolverBase2(const CUDAParticleSystemSolverBase2&) =
        delete;

    //! Deleted move constructor.
    CUDAParticleSystemSolverBase2(CUDAParticleSystemSolverBase2&&) noexcept =
        delete;

    //! Virtual default destructor.
    ~CUDAParticleSystemSolverBase2() override = default;

    //! Deleted copy assignment operator.
    CUDAParticleSystemSolverBase2& operator=(
        const CUDAParticleSystemSolverBase2&) = delete;

    //! Deleted move assignment operator.
    CUDAParticleSystemSolverBase2& operator=(
        CUDAParticleSystemSolverBase2&&) noexcept = delete;

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
    const Vector2F& Gravity() const;

    //! Sets the gravity.
    void SetGravity(const Vector2F& newGravity);

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    virtual CUDAParticleSystemData2* ParticleSystemData();

    //!
    //! \brief Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    virtual const CUDAParticleSystemData2* ParticleSystemData() const;

 protected:
    //! Initializes the simulator.
    void OnInitialize() override;

    void UpdateCollider(double timeStepInSeconds);

    void UpdateEmitter(double timeStepInSeconds);

 private:
    float m_dragCoefficient = 1e-4f;
    float m_restitutionCoefficient = 0.0f;
    Vector2F m_gravity{ 0.0f, GRAVITY_FLOAT };
    CUDAParticleSystemData2Ptr m_particleSystemData;
};

//! Shared pointer type for the CUDAParticleSystemSolverBase2.
using CUDAParticleSystemSolverBase2Ptr =
    std::shared_ptr<CUDAParticleSystemSolverBase2>;

//!
//! \brief Base class for CUDA-based particle solver builder.
//!
template <typename DerivedBuilder>
class CUDAParticleSystemSolverBuilderBase2
{
 public:
    //! Returns builder with particle drag coefficient.
    DerivedBuilder& WithDragCoefficient(float coeff);

    //! Returns builder with particle restitution coefficient.
    DerivedBuilder& WithRestitutionCoefficient(float coeff);

    //! Returns builder with particle gravity.
    DerivedBuilder& WithGravity(const Vector2F& gravity);

 protected:
    float m_dragCoefficient = 1e-4f;
    float m_restitutionCoefficient = 0.0f;
    Vector2F m_gravity{ 0.0f, GRAVITY_FLOAT };
};

template <typename T>
T& CUDAParticleSystemSolverBuilderBase2<T>::WithDragCoefficient(float coeff)
{
    m_dragCoefficient = coeff;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDAParticleSystemSolverBuilderBase2<T>::WithRestitutionCoefficient(
    float coeff)
{
    m_restitutionCoefficient = coeff;
    return static_cast<T&>(*this);
}

template <typename T>
T& CUDAParticleSystemSolverBuilderBase2<T>::WithGravity(const Vector2F& gravity)
{
    m_gravity = gravity;
    return static_cast<T&>(*this);
}
}  // namespace CubbyFlow

#endif

#endif