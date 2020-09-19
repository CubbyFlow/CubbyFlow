// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_PARTICLE_EMITTER2_HPP
#define CUBBYFLOW_POINT_PARTICLE_EMITTER2_HPP

#include <Core/Emitter/ParticleEmitter2.hpp>

#include <random>

namespace CubbyFlow
{
//!
//! \brief 2-D point particle emitter.
//!
//! This class emits particles from a single point in given direction, speed,
//! and spreading angle.
//!
class PointParticleEmitter2 final : public ParticleEmitter2
{
 public:
    class Builder;

    //!
    //! Constructs an emitter that spawns particles from given origin,
    //! direction, speed, spread angle, max number of new particles per second,
    //! max total number of particles to be emitted, and random seed.
    //!
    //! \param[in]  origin                      The origin.
    //! \param[in]  direction                   The direction.
    //! \param[in]  speed                       The speed.
    //! \param[in]  spreadAngleInDegrees        The spread angle in degrees.
    //! \param[in]  maxNumOfNewParticlesPerSec  The max number of new particles
    //!                                         per second.
    //! \param[in]  maxNumOfParticles           The max number of particles to
    //!                                         be emitted.
    //! \param[in]  seed                        The random seed.
    //!
    PointParticleEmitter2(
        const Vector2D& origin, const Vector2D& direction, double speed,
        double spreadAngleInDegrees, size_t maxNumOfNewParticlesPerSec = 1,
        size_t maxNumOfParticles = std::numeric_limits<size_t>::max(),
        uint32_t seed = 0);

    //! Returns max number of new particles per second.
    [[nodiscard]] size_t GetMaxNumberOfNewParticlesPerSecond() const;

    //! Sets max number of new particles per second.
    void SetMaxNumberOfNewParticlesPerSecond(size_t rate);

    //! Returns max number of particles to be emitted.
    [[nodiscard]] size_t GetMaxNumberOfParticles() const;

    //! Sets max number of particles to be emitted.
    void SetMaxNumberOfParticles(size_t maxNumberOfParticles);

    //! Returns builder fox PointParticleEmitter2.
    static Builder GetBuilder();

 private:
    //!
    //! \brief      Emits particles to the particle system data.
    //!
    //! \param[in]  currentTimeInSeconds    Current simulation time.
    //! \param[in]  timeIntervalInSeconds   The time-step interval.
    //!
    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSeconds) override;

    void Emit(Array1<Vector2D>* newPositions, Array1<Vector2D>* newVelocities,
              size_t maxNewNumberOfParticles);

    [[nodiscard]] double Random();

    std::mt19937 m_rng;

    double m_firstFrameTimeInSeconds = 0.0;
    size_t m_numberOfEmittedParticles = 0;

    size_t m_maxNumberOfNewParticlesPerSecond;
    size_t m_maxNumberOfParticles;

    Vector2D m_origin;
    Vector2D m_direction;
    double m_speed;
    double m_spreadAngleInRadians;
};

//! Shared pointer for the PointParticleEmitter2 type.
using PointParticleEmitter2Ptr = std::shared_ptr<PointParticleEmitter2>;

//!
//! \brief Front-end to create PointParticleEmitter2 objects step by step.
//!
class PointParticleEmitter2::Builder final
{
 public:
    //! Returns builder with origin.
    [[nodiscard]] Builder& WithOrigin(const Vector2D& origin);

    //! Returns builder with direction.
    [[nodiscard]] Builder& WithDirection(const Vector2D& direction);

    //! Returns builder with speed.
    [[nodiscard]] Builder& WithSpeed(double speed);

    //! Returns builder with spread angle in degrees.
    [[nodiscard]] Builder& WithSpreadAngleInDegrees(
        double spreadAngleInDegrees);

    [[nodiscard]] Builder& WithMaxNumberOfNewParticlesPerSecond(
        size_t maxNumOfNewParticlesPerSec);

    //! Returns builder with max number of particles.
    [[nodiscard]] Builder& WithMaxNumberOfParticles(
        size_t maxNumberOfParticles);

    //! Returns builder with random seed.
    [[nodiscard]] Builder& WithRandomSeed(uint32_t seed);

    //! Builds PointParticleEmitter2.
    [[nodiscard]] PointParticleEmitter2 Build() const;

    //! Builds shared pointer of PointParticleEmitter2 instance.
    [[nodiscard]] PointParticleEmitter2Ptr MakeShared() const;

 private:
    size_t m_maxNumberOfNewParticlesPerSecond = 1;
    size_t m_maxNumberOfParticles = std::numeric_limits<size_t>::max();
    Vector2D m_origin{ 0, 0 };
    Vector2D m_direction{ 0, 1 };
    double m_speed = 1.0;
    double m_spreadAngleInDegrees = 90.0;
    uint32_t m_seed = 0;
};
}  // namespace CubbyFlow

#endif