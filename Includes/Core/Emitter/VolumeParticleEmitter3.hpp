// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VOLUME_PARTICLE_EMITTER3_HPP
#define CUBBYFLOW_VOLUME_PARTICLE_EMITTER3_HPP

#include <Core/Emitter/ParticleEmitter3.hpp>
#include <Core/PointGenerator/PointGenerator3.hpp>
#include <Core/Surface/ImplicitSurface3.hpp>

#include <random>

namespace CubbyFlow
{
//!
//! \brief 3-D volumetric particle emitter.
//!
//! This class emits particles from volumetric geometry.
//!
class VolumeParticleEmitter3 final : public ParticleEmitter3
{
 public:
    class Builder;

    //!
    //! Constructs an emitter that spawns particles from given implicit surface
    //! which defines the volumetric geometry. Provided bounding box limits
    //! the particle generation region.
    //!
    //! \param[in]  implicitSurface         The implicit surface.
    //! \param[in]  maxRegion               The max region.
    //! \param[in]  spacing                 The spacing between particles.
    //! \param[in]  initialVel              The initial velocity.
    //! \param[in]  maxNumberOfParticles    The max number of particles to be
    //!                                     emitted.
    //! \param[in]  jitter                  The jitter amount between 0 and 1.
    //! \param[in]  isOneShot               True if emitter gets disabled after
    //!                                     one shot.
    //! \param[in]  allowOverlapping        True if particles can be overlapped.
    //! \param[in]  seed                    The random seed.
    //!
    VolumeParticleEmitter3(
        const ImplicitSurface3Ptr& implicitSurface,
        const BoundingBox3D& maxRegion, double spacing,
        const Vector3D& initialVel = Vector3D(),
        const Vector3D& linearVel = Vector3D(),
        const Vector3D& angularVel = Vector3D(),
        size_t maxNumberOfParticles = std::numeric_limits<size_t>::max(),
        double jitter = 0.0, bool isOneShot = true,
        bool allowOverlapping = false, uint32_t seed = 0);

    //!
    //! \brief      Sets the point generator.
    //!
    //! This function sets the point generator that defines the pattern of the
    //! point distribution within the volume.
    //!
    //! \param[in]  newPointsGen The new points generator.
    //!
    void SetPointGenerator(const PointGenerator3Ptr& newPointsGen);

    //! Returns source surface.
    const ImplicitSurface3Ptr& GetSurface() const;

    //! Sets the source surface.
    void SetSurface(const ImplicitSurface3Ptr& newSurface);

    //! Returns max particle gen region.
    const BoundingBox3D& GetMaxRegion() const;

    //! Sets the max particle gen region.
    void SetMaxRegion(const BoundingBox3D& newMaxRegion);

    //! Returns jitter amount.
    double GetJitter() const;

    //! Sets jitter amount between 0 and 1.
    void SetJitter(double newJitter);

    //! Returns true if particles should be emitted just once.
    bool GetIsOneShot() const;

    //!
    //! \brief      Sets the flag to true if particles are emitted just once.
    //!
    //! If true is set, the emitter will generate particles only once even after
    //! multiple emit calls. If false, it will keep generating particles from
    //! the volumetric geometry. Default value is true.
    //!
    //! \param[in]  newValue True if particles should be emitted just once.
    //!
    void SetIsOneShot(bool newValue);

    //! Returns true if particles can be overlapped.
    bool GetAllowOverlapping() const;

    //!
    //! \brief      Sets the flag to true if particles can overlap each other.
    //!
    //! If true is set, the emitter will generate particles even if the new
    //! particles can find existing nearby particles within the particle
    //! spacing.
    //!
    //! \param[in]  newValue True if particles can be overlapped.
    //!
    void SetAllowOverlapping(bool newValue);

    //! Returns max number of particles to be emitted.
    size_t GetMaxNumberOfParticles() const;

    //! Sets the max number of particles to be emitted.
    void SetMaxNumberOfParticles(size_t newMaxNumberOfParticles);

    //! Returns the spacing between particles.
    double GetSpacing() const;

    //! Sets the spacing between particles.
    void SetSpacing(double newSpacing);

    //! Sets the initial velocity of the particles.
    Vector3D GetInitialVelocity() const;

    //! Returns the initial velocity of the particles.
    void SetInitialVelocity(const Vector3D& newInitialVel);

    //! Returns the linear velocity of the emitter.
    Vector3D GetLinearVelocity() const;

    //! Sets the linear velocity of the emitter.
    void SetLinearVelocity(const Vector3D& newLinearVel);

    //! Returns the angular velocity of the emitter.
    Vector3D GetAngularVelocity() const;

    //! Sets the linear velocity of the emitter.
    void SetAngularVelocity(const Vector3D& newAngularVel);

    //! Returns builder fox VolumeParticleEmitter3.
    static Builder GetBuilder();

 private:
    std::mt19937 m_rng;

    ImplicitSurface3Ptr m_implicitSurface;
    BoundingBox3D m_maxRegion;
    double m_spacing;
    Vector3D m_initialVel;
    Vector3D m_linearVel;
    Vector3D m_angularVel;
    PointGenerator3Ptr m_pointsGen;

    size_t m_maxNumberOfParticles = std::numeric_limits<size_t>::max();
    size_t m_numberOfEmittedParticles = 0;

    double m_jitter = 0.0;
    bool m_isOneShot = true;
    bool m_allowOverlapping = false;

    //!
    //! \brief      Emits particles to the particle system data.
    //!
    //! \param[in]  currentTimeInSeconds    Current simulation time.
    //! \param[in]  timeIntervalInSeconds   The time-step interval.
    //!
    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSeconds) override;

    void Emit(const ParticleSystemData3Ptr& particles,
              Array1<Vector3D>* newPositions, Array1<Vector3D>* newVelocities);

    double Random();

    Vector3D VelocityAt(const Vector3D& point) const;
};

//! Shared pointer for the VolumeParticleEmitter3 type.
using VolumeParticleEmitter3Ptr = std::shared_ptr<VolumeParticleEmitter3>;

//!
//! \brief Front-end to create VolumeParticleEmitter3 objects step by step.
//!
class VolumeParticleEmitter3::Builder final
{
 public:
    //! Returns builder with implicit surface defining volume shape.
    Builder& WithImplicitSurface(const ImplicitSurface3Ptr& implicitSurface);

    //! Returns builder with surface defining volume shape.
    Builder& WithSurface(const Surface3Ptr& surface);

    //! Returns builder with max region.
    Builder& WithMaxRegion(const BoundingBox3D& maxRegion);

    //! Returns builder with spacing.
    Builder& WithSpacing(double spacing);

    //! Returns builder with initial velocity.
    Builder& WithInitialVelocity(const Vector3D& initialVel);

    //! Returns builder with linear velocity.
    Builder& WithLinearVelocity(const Vector3D& linearVel);

    //! Returns builder with angular velocity.
    Builder& WithAngularVelocity(const Vector3D& angularVel);

    //! Returns builder with max number of particles.
    Builder& WithMaxNumberOfParticles(size_t maxNumberOfParticles);

    //! Returns builder with jitter amount.
    Builder& WithJitter(double jitter);

    //! Returns builder with one-shot flag.
    Builder& WithIsOneShot(bool isOneShot);

    //! Returns builder with overlapping flag.
    Builder& WithAllowOverlapping(bool allowOverlapping);

    //! Returns builder with random seed.
    Builder& WithRandomSeed(uint32_t seed);

    //! Builds VolumeParticleEmitter3.
    VolumeParticleEmitter3 Build() const;

    //! Builds shared pointer of VolumeParticleEmitter3 instance.
    VolumeParticleEmitter3Ptr MakeShared() const;

 private:
    ImplicitSurface3Ptr m_implicitSurface;
    bool m_isBoundSet = false;
    BoundingBox3D m_maxRegion;
    double m_spacing = 0.1;
    Vector3D m_initialVel;
    Vector3D m_linearVel;
    Vector3D m_angularVel;
    size_t m_maxNumberOfParticles = std::numeric_limits<size_t>::max();
    double m_jitter = 0.0;
    bool m_isOneShot = true;
    bool m_allowOverlapping = false;
    uint32_t m_seed = 0;
};
}  // namespace CubbyFlow

#endif