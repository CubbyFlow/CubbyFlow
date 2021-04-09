// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_SYSTEM_SOLVER2_HPP
#define CUBBYFLOW_PARTICLE_SYSTEM_SOLVER2_HPP

#include <Core/Animation/PhysicsAnimation.hpp>
#include <Core/Emitter/ParticleEmitter2.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Geometry/Collider.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Particle/ParticleSystemData.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief      Basic 2-D particle system solver.
//!
//! This class implements basic particle system solver. It includes gravity,
//! air drag, and collision. But it does not compute particle-to-particle
//! interaction. Thus, this solver is suitable for performing simple spray-like
//! simulations with low computational cost. This class can be further extend
//! to add more sophisticated simulations, such as SPH, to handle
//! particle-to-particle intersection.
//!
//! \see        SPHSolver2
//!
class ParticleSystemSolver2 : public PhysicsAnimation
{
 public:
    class Builder;

    //! Constructs an empty solver.
    ParticleSystemSolver2();

    //! Constructs a solver with particle parameters.
    ParticleSystemSolver2(double radius, double mass);

    //! Deleted copy constructor.
    ParticleSystemSolver2(const ParticleSystemSolver2&) = delete;

    //! Deleted move constructor.
    ParticleSystemSolver2(ParticleSystemSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~ParticleSystemSolver2() override = default;

    //! Deleted copy assignment operator.
    ParticleSystemSolver2& operator=(const ParticleSystemSolver2&) = delete;

    //! Deleted move assignment operator.
    ParticleSystemSolver2& operator=(ParticleSystemSolver2&&) noexcept = delete;

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
    [[nodiscard]] const Vector2D& GetGravity() const;

    //! Sets the gravity.
    void SetGravity(const Vector2D& newGravity);

    //!
    //! \brief      Returns the particle system data.
    //!
    //! This function returns the particle system data. The data is created when
    //! this solver is constructed and also owned by the solver.
    //!
    //! \return     The particle system data.
    //!
    [[nodiscard]] const ParticleSystemData2Ptr& GetParticleSystemData() const;

    //! Returns the collider.
    [[nodiscard]] const Collider2Ptr& GetCollider() const;

    //! Sets the collider.
    void SetCollider(const Collider2Ptr& newCollider);

    //! Returns the emitter.
    [[nodiscard]] const ParticleEmitter2Ptr& GetEmitter() const;

    //! Sets the emitter.
    void SetEmitter(const ParticleEmitter2Ptr& newEmitter);

    //! Returns the wind field.
    [[nodiscard]] const VectorField2Ptr& GetWind() const;

    //!
    //! \brief      Sets the wind.
    //!
    //! Wind can be applied to the particle system by setting a vector field to
    //! the solver.
    //!
    //! \param[in]  newWind The new wind.
    //!
    void SetWind(const VectorField2Ptr& newWind);

    //! Returns builder fox ParticleSystemSolver2.
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
    void ResolveCollision(ArrayView1<Vector2D> newPositions,
                          ArrayView1<Vector2D> newVelocities);

    //! Assign a new particle system data.
    void SetParticleSystemData(const ParticleSystemData2Ptr& newParticles);

 private:
    void BeginAdvanceTimeStep(double timeStepInSeconds);

    void EndAdvanceTimeStep(double timeStepInSeconds);

    void AccumulateExternalForces();

    void TimeIntegration(double timeStepInSeconds);

    void UpdateCollider(double timeStepInSeconds) const;

    void UpdateEmitter(double timeStepInSeconds) const;

    double m_dragCoefficient = 1e-4;
    double m_restitutionCoefficient = 0.0;
    Vector2D m_gravity = Vector2D{ 0.0, GRAVITY };

    ParticleSystemData2Ptr m_particleSystemData;
    ParticleSystemData2::VectorData m_newPositions;
    ParticleSystemData2::VectorData m_newVelocities;
    Collider2Ptr m_collider;
    ParticleEmitter2Ptr m_emitter;
    VectorField2Ptr m_wind;
};

//! Shared pointer type for the ParticleSystemSolver2.
using ParticleSystemSolver2Ptr = std::shared_ptr<ParticleSystemSolver2>;

//!
//! \brief Base class for particle-based solver builder.
//!
template <typename DerivedBuilder>
class ParticleSystemSolverBuilderBase2
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
T& ParticleSystemSolverBuilderBase2<T>::WithRadius(double radius)
{
    m_radius = radius;
    return static_cast<T&>(*this);
}

template <typename T>
T& ParticleSystemSolverBuilderBase2<T>::WithMass(double mass)
{
    m_mass = mass;
    return static_cast<T&>(*this);
}

//!
//! \brief Front-end to create ParticleSystemSolver2 objects step by step.
//!
class ParticleSystemSolver2::Builder final
    : public ParticleSystemSolverBuilderBase2<Builder>
{
 public:
    //! Builds ParticleSystemSolver2.
    [[nodiscard]] ParticleSystemSolver2 Build() const;

    //! Builds shared pointer of ParticleSystemSolver2 instance.
    [[nodiscard]] ParticleSystemSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif