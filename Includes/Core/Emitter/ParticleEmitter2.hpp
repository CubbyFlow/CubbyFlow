// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PARTICLE_EMITTER2_HPP
#define CUBBYFLOW_PARTICLE_EMITTER2_HPP

#include <Core/Particle/ParticleSystemData.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D particle emitter.
//!
class ParticleEmitter2
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the emitter pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(ParticleEmitter2*, double, double)>;

    //! Default constructor.
    ParticleEmitter2() = default;

    //! Default copy constructor.
    ParticleEmitter2(const ParticleEmitter2&) = default;

    //! Default move constructor.
    ParticleEmitter2(ParticleEmitter2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~ParticleEmitter2() = default;

    //! Default copy assignment operator.
    ParticleEmitter2& operator=(const ParticleEmitter2&) = default;

    //! Default move assignment operator.
    ParticleEmitter2& operator=(ParticleEmitter2&&) noexcept = default;

    //! Updates the emitter state from \p currentTimeInSeconds to the following
    //! time-step.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //! Returns the target particle system to emit.
    [[nodiscard]] const ParticleSystemData2Ptr& GetTarget() const;

    //! Sets the target particle system to emit.
    void SetTarget(const ParticleSystemData2Ptr& particles);

    //! Returns true if the emitter is enabled.
    [[nodiscard]] bool GetIsEnabled() const;

    //! Sets true/false to enable/disable the emitter.
    void SetIsEnabled(bool enabled);

    //!
    //! \brief      Sets the callback function to be called when
    //!             ParticleEmitter2::Update function is invoked.
    //!
    //! The callback function takes current simulation time in seconds unit. Use
    //! this callback to track any motion or state changes related to this
    //! emitter.
    //!
    //! \param[in]  callback The callback function.
    //!
    void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

 protected:
    //! Called when ParticleEmitter2::SetTarget is executed.
    virtual void OnSetTarget(const ParticleSystemData2Ptr& particles);

    //! Called when ParticleEmitter2::Update is executed.
    virtual void OnUpdate(double currentTimeInSeconds,
                          double timeIntervalInSeconds) = 0;

 private:
    ParticleSystemData2Ptr m_particles;
    OnBeginUpdateCallback m_onBeginUpdateCallback;
    bool m_isEnabled = true;
};

//! Shared pointer for the ParticleEmitter2 type.
using ParticleEmitter2Ptr = std::shared_ptr<ParticleEmitter2>;
}  // namespace CubbyFlow

#endif