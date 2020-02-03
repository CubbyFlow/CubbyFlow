// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/Macros.h>
#include <Core/Emitter/ParticleEmitter3.hpp>

namespace CubbyFlow
{
ParticleEmitter3::ParticleEmitter3()
{
    // Do nothing
}

ParticleEmitter3::~ParticleEmitter3()
{
    // Do nothing
}

const ParticleSystemData3Ptr& ParticleEmitter3::GetTarget() const
{
    return m_particles;
}

void ParticleEmitter3::SetTarget(const ParticleSystemData3Ptr& particles)
{
    m_particles = particles;

    OnSetTarget(particles);
}

void ParticleEmitter3::Update(double currentTimeInSeconds,
                              double timeIntervalInSeconds)
{
    if (m_onBeginUpdateCallback)
    {
        m_onBeginUpdateCallback(this, currentTimeInSeconds,
                                timeIntervalInSeconds);
    }

    OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

void ParticleEmitter3::OnSetTarget(const ParticleSystemData3Ptr& particles)
{
    UNUSED_VARIABLE(particles);
}

void ParticleEmitter3::SetOnBeginUpdateCallback(
    const OnBeginUpdateCallback& callback)
{
    m_onBeginUpdateCallback = callback;
}
}  // namespace CubbyFlow
