// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Emitter/ParticleEmitter2.hpp>
#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
void ParticleEmitter2::Update(double currentTimeInSeconds,
                              double timeIntervalInSeconds)
{
    if (m_onBeginUpdateCallback)
    {
        m_onBeginUpdateCallback(this, currentTimeInSeconds,
                                timeIntervalInSeconds);
    }

    OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

const ParticleSystemData2Ptr& ParticleEmitter2::GetTarget() const
{
    return m_particles;
}

void ParticleEmitter2::SetTarget(const ParticleSystemData2Ptr& particles)
{
    m_particles = particles;

    OnSetTarget(particles);
}

bool ParticleEmitter2::GetIsEnabled() const
{
    return m_isEnabled;
}

void ParticleEmitter2::SetIsEnabled(bool enabled)
{
    m_isEnabled = enabled;
}

void ParticleEmitter2::SetOnBeginUpdateCallback(
    const OnBeginUpdateCallback& callback)
{
    m_onBeginUpdateCallback = callback;
}

void ParticleEmitter2::OnSetTarget(const ParticleSystemData2Ptr& particles)
{
    UNUSED_VARIABLE(particles);
}
}  // namespace CubbyFlow
