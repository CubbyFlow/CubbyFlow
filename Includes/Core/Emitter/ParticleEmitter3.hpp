// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_EMITTER3_HPP
#define CUBBYFLOW_PARTICLE_EMITTER3_HPP

#include <Core/Particle/ParticleSystemData.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 3-D particle emitter.
//!
class ParticleEmitter3
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the emitter pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(ParticleEmitter3*, double, double)>;

    //! Default constructor.
    ParticleEmitter3() = default;

    //! Default copy constructor.
    ParticleEmitter3(const ParticleEmitter3&) = default;

    //! Default move constructor.
    ParticleEmitter3(ParticleEmitter3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~ParticleEmitter3() = default;

    //! Default copy assignment operator.
    ParticleEmitter3& operator=(const ParticleEmitter3&) = default;

    //! Default move assignment operator.
    ParticleEmitter3& operator=(ParticleEmitter3&&) noexcept = default;

    //! Updates the emitter state from \p currentTimeInSeconds to the following
    //! time-step.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //! Returns the target particle system to emit.
    [[nodiscard]] const ParticleSystemData3Ptr& GetTarget() const;

    //! Sets the target particle system to emit.
    void SetTarget(const ParticleSystemData3Ptr& particles);

    //! Returns true if the emitter is enabled.
    [[nodiscard]] bool GetIsEnabled() const;

    //! Sets true/false to enable/disable the emitter.
    void SetIsEnabled(bool enabled);

    //!
    //! \brief      Sets the callback function to be called when
    //!             ParticleEmitter3::Update function is invoked.
    //!
    //! The callback function takes current simulation time in seconds unit. Use
    //! this callback to track any motion or state changes related to this
    //! emitter.
    //!
    //! \param[in]  callback The callback function.
    //!
    void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

 protected:
    //! Called when ParticleEmitter3::SetTarget is executed.
    virtual void OnSetTarget(const ParticleSystemData3Ptr& particles);

    //! Called when ParticleEmitter3::Update is executed.
    virtual void OnUpdate(double currentTimeInSeconds,
                          double timeIntervalInSeconds) = 0;

 private:
    ParticleSystemData3Ptr m_particles;
    OnBeginUpdateCallback m_onBeginUpdateCallback;
    bool m_isEnabled = true;
};

//! Shared pointer for the ParticleEmitter3 type.
using ParticleEmitter3Ptr = std::shared_ptr<ParticleEmitter3>;
}  // namespace CubbyFlow

#endif