// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_SYSTEM_SOLVER3_HPP
#define CUBBYFLOW_PARTICLE_SYSTEM_SOLVER3_HPP

#include <Core/Animation/PhysicsAnimation.hpp>
#include <Core/Emitter/ParticleEmitter3.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Geometry/Collider.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Particle/ParticleSystemData.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief      Basic 3-D particle system solver.
//!
//! This class implements basic particle system solver. It includes gravity,
//! air drag, and collision. But it does not compute particle-to-particle
//! interaction. Thus, this solver is suitable for performing simple spray-like
//! simulations with low computational cost. This class can be further extend
//! to add more sophisticated simulations, such as SPH, to handle
//! particle-to-particle intersection.
//!
//! \see        SPHSolver3
//!
class ParticleSystemSolver3 : public PhysicsAnimation
{
 public:
    class Builder;

    //! Constructs an empty solver.
    ParticleSystemSolver3();

    //! Constructs a solver with particle parameters.
    ParticleSystemSolver3(double radius, double mass);

    //! Deleted copy constructor.
    ParticleSystemSolver3(const ParticleSystemSolver3&) = delete;

    //! Deleted move constructor.
    ParticleSystemSolver3(ParticleSystemSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~ParticleSystemSolver3() override = default;

    //! Deleted copy assignment operator.
    ParticleSystemSolver3& operator=(const ParticleSystemSolver3&) = delete;

    //! Deleted move assignment operator.
    ParticleSystemSolver3& operator=(ParticleSystemSolver3&&) noexcept = delete;

    //! Returns the drag coefficient.
    [[nodiscard]] double GetDragCoefficient() const;

    //!
    //! \brief      Sets the drag coefficient.
    //!
    //! The drag coefficient controls the amount of air-drag. The coefficient
    //! should be a positive number and 0 means no drag force.
    //!
    //! \param[in]  newDragCoefficient The new drag coefficient.
    //!
    void SetDragCoefficient(double newDragCoefficient);

    //! Gets the restitution coefficient.
    [[nodiscard]] double GetRestitutionCoefficient() const;

    //!
    //! \brief      Sets the restitution coefficient.
    //!
    //! The restitution coefficient controls the bounciness of a particle when
    //! it hits a collider surface. The range of the coefficient should be 0 to
    //! 1 -- 0 means no bounce back and 1 means perfect reflection.
    //!
    //! \param[in]  newRestitutionCoefficient The new restitution coefficient.
    //!
    void SetRestitutionCoefficient(double newRestitutionCoefficient);

    //! Returns the gravity.
    [[nodiscard]] const Vector3D& GetGravity() const;

    //! Sets the gravity.
    void SetGravity(const Vector3D& newGravity);

    //!
    //! \brief      Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    //! \return     The particle system data.
    //!
    [[nodiscard]] const ParticleSystemData3Ptr& GetParticleSystemData() const;

    //! Returns the collider.
    [[nodiscard]] const Collider3Ptr& GetCollider() const;

    //! Sets the collider.
    void SetCollider(const Collider3Ptr& newCollider);

    //! Returns the emitter.
    [[nodiscard]] const ParticleEmitter3Ptr& GetEmitter() const;

    //! Sets the emitter.
    void SetEmitter(const ParticleEmitter3Ptr& newEmitter);

    //! Returns the wind field.
    [[nodiscard]] const VectorField3Ptr& GetWind() const;

    //!
    //! \brief      Sets the wind.
    //!
    //! Wind can be applied to the particle system by setting a vector field to
    //! the solver.
    //!
    //! \param[in]  newWind The new wind.
    //!
    void SetWind(const VectorField3Ptr& newWind);

    //! Returns builder fox ParticleSystemSolver3.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Initializes the simulator.
    void OnInitialize() override;

    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

    //! Accumulates forces applied to the particles.
    virtual void AccumulateForces(double timeStepInSeconds);

    //! Called when a time-step is about to begin.
    virtual void OnBeginAdvanceTimeStep(double timeStepInSeconds);

    //! Called after a time-step is completed.
    virtual void OnEndAdvanceTimeStep(double timeStepInSeconds);

    //! Resolves any collisions occurred by the particles.
    void ResolveCollision();

    //! Resolves any collisions occurred by the particles where the particle
    //! state is given by the position and velocity arrays.
    void ResolveCollision(ArrayView1<Vector3D> newPositions,
                          ArrayView1<Vector3D> newVelocities);

    //! Assign a new particle system data.
    void SetParticleSystemData(const ParticleSystemData3Ptr& newParticles);

 private:
    void BeginAdvanceTimeStep(double timeStepInSeconds);

    void EndAdvanceTimeStep(double timeStepInSeconds);

    void AccumulateExternalForces();

    void TimeIntegration(double timeStepInSeconds);

    void UpdateCollider(double timeStepInSeconds) const;

    void UpdateEmitter(double timeStepInSeconds) const;

    double m_dragCoefficient = 1e-4;
    double m_restitutionCoefficient = 0.0;
    Vector3D m_gravity = Vector3D{ 0.0, GRAVITY, 0.0 };

    ParticleSystemData3Ptr m_particleSystemData;
    ParticleSystemData3::VectorData m_newPositions;
    ParticleSystemData3::VectorData m_newVelocities;
    Collider3Ptr m_collider;
    ParticleEmitter3Ptr m_emitter;
    VectorField3Ptr m_wind;
};

//! Shared pointer type for the ParticleSystemSolver3.
using ParticleSystemSolver3Ptr = std::shared_ptr<ParticleSystemSolver3>;

//!
//! \brief Base class for particle-based solver builder.
//!
template <typename DerivedBuilder>
class ParticleSystemSolverBuilderBase3
{
 public:
    //! Returns builder with particle radius.
    [[nodiscard]] DerivedBuilder& WithRadius(double radius);

    //! Returns builder with mass per particle.
    [[nodiscard]] DerivedBuilder& WithMass(double mass);

 protected:
    double m_radius = 1e-3;
    double m_mass = 1e-3;
};

template <typename T>
T& ParticleSystemSolverBuilderBase3<T>::WithRadius(double radius)
{
    m_radius = radius;
    return static_cast<T&>(*this);
}

template <typename T>
T& ParticleSystemSolverBuilderBase3<T>::WithMass(double mass)
{
    m_mass = mass;
    return static_cast<T&>(*this);
}

//!
//! \brief Front-end to create ParticleSystemSolver3 objects step by step.
//!
class ParticleSystemSolver3::Builder final
    : public ParticleSystemSolverBuilderBase3<Builder>
{
 public:
    //! Builds ParticleSystemSolver3.
    [[nodiscard]] ParticleSystemSolver3 Build() const;

    //! Builds shared pointer of ParticleSystemSolver3 instance.
    [[nodiscard]] ParticleSystemSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif