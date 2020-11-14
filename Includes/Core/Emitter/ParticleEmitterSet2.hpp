// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_EMITTER_SET2_HPP
#define CUBBYFLOW_PARTICLE_EMITTER_SET2_HPP

#include <Core/Emitter/ParticleEmitter2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D particle-based emitter set.
//!
class ParticleEmitterSet2 final : public ParticleEmitter2
{
 public:
    class Builder;

    //! Default constructor.
    ParticleEmitterSet2() = default;

    //! Constructs an emitter with sub-emitters.
    explicit ParticleEmitterSet2(std::vector<ParticleEmitter2Ptr> emitters);

    //! Default copy constructor.
    ParticleEmitterSet2(const ParticleEmitterSet2&) = default;

    //! Default move constructor.
    ParticleEmitterSet2(ParticleEmitterSet2&&) noexcept = default;

    //! Default virtual destructor.
    ~ParticleEmitterSet2() override = default;

    //! Default copy assignment operator.
    ParticleEmitterSet2& operator=(const ParticleEmitterSet2&) = default;

    //! Default move assignment operator.
    ParticleEmitterSet2& operator=(ParticleEmitterSet2&&) noexcept = default;

    //! Adds sub-emitter.
    void AddEmitter(const ParticleEmitter2Ptr& emitter);

    //! Returns builder fox ParticleEmitterSet2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::vector<ParticleEmitter2Ptr> m_emitters;

    void OnSetTarget(const ParticleSystemData2Ptr& particles) override;

    void OnUpdate(double currentTimeInSeconds,
                  double timeIntervalInSecond) override;
};

//! Shared pointer type for the ParticleEmitterSet2.
using ParticleEmitterSet2Ptr = std::shared_ptr<ParticleEmitterSet2>;

//!
//! \brief Front-end to create ParticleEmitterSet2 objects step by step.
//!
class ParticleEmitterSet2::Builder final
{
 public:
    //! Returns builder with list of sub-emitters.
    [[nodiscard]] Builder& WithEmitters(
        const std::vector<ParticleEmitter2Ptr>& emitters);

    //! Builds ParticleEmitterSet2.
    [[nodiscard]] ParticleEmitterSet2 Build() const;

    //! Builds shared pointer of ParticleEmitterSet2 instance.
    [[nodiscard]] ParticleEmitterSet2Ptr MakeShared() const;

 private:
    std::vector<ParticleEmitter2Ptr> m_emitters;
};
}  // namespace CubbyFlow

#endif