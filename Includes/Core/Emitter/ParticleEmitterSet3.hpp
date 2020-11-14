// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_EMITTER_SET3_HPP
#define CUBBYFLOW_PARTICLE_EMITTER_SET3_HPP

#include <Core/Emitter/ParticleEmitter3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D particle-based emitter set.
//!
class ParticleEmitterSet3 final : public ParticleEmitter3
{
 public:
    class Builder;

    //! Default constructor.
    ParticleEmitterSet3() = default;

    //! Constructs an emitter with sub-emitters.
    explicit ParticleEmitterSet3(std::vector<ParticleEmitter3Ptr> emitters);

    //! Default copy constructor.
    ParticleEmitterSet3(const ParticleEmitterSet3&) = default;

    //! Default move constructor.
    ParticleEmitterSet3(ParticleEmitterSet3&&) noexcept = default;

    //! Default virtual destructor.
    ~ParticleEmitterSet3() override = default;

    //! Default copy assignment operator.
    ParticleEmitterSet3& operator=(const ParticleEmitterSet3&) = default;

    //! Default move assignment operator.
    ParticleEmitterSet3& operator=(ParticleEmitterSet3&&) noexcept = default;

    //! Adds sub-emitter.
    void AddEmitter(const ParticleEmitter3Ptr& emitter);

    //! Returns builder fox ParticleEmitterSet3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::vector<ParticleEmitter3Ptr> m_emitters;

    void OnSetTarget(const ParticleSystemData3Ptr& particles) override;

    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSecond) override;
};

//! Shared pointer type for the ParticleEmitterSet3.
using ParticleEmitterSet3Ptr = std::shared_ptr<ParticleEmitterSet3>;

//!
//! \brief Front-end to create ParticleEmitterSet3 objects step by step.
//!
class ParticleEmitterSet3::Builder final
{
 public:
    //! Returns builder with list of sub-emitters.
    [[nodiscard]] Builder& WithEmitters(
        const std::vector<ParticleEmitter3Ptr>& emitters);

    //! Builds ParticleEmitterSet3.
    [[nodiscard]] ParticleEmitterSet3 Build() const;

    //! Builds shared pointer of ParticleEmitterSet3 instance.
    [[nodiscard]] ParticleEmitterSet3Ptr MakeShared() const;

 private:
    std::vector<ParticleEmitter3Ptr> m_emitters;
};
}  // namespace CubbyFlow

#endif