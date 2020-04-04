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

    //! Constructs an emitter.
    ParticleEmitterSet2();

    //! Constructs an emitter with sub-emitters.
    explicit ParticleEmitterSet2(
        const std::vector<ParticleEmitter2Ptr>& emitters);

    //! Destructor.
    virtual ~ParticleEmitterSet2();

    //! Adds sub-emitter.
    void AddEmitter(const ParticleEmitter2Ptr& emitter);

    //! Returns builder fox ParticleEmitterSet2.
    static Builder GetBuilder();

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
    Builder& WithEmitters(const std::vector<ParticleEmitter2Ptr>& emitters);

    //! Builds ParticleEmitterSet2.
    ParticleEmitterSet2 Build() const;

    //! Builds shared pointer of ParticleEmitterSet2 instance.
    ParticleEmitterSet2Ptr MakeShared() const;

 private:
    std::vector<ParticleEmitter2Ptr> m_emitters;
};
}  // namespace CubbyFlow

#endif