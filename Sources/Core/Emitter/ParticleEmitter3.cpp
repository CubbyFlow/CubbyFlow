/*************************************************************************
> File Name: ParticleEmitter3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D particle emitter.
> Created Time: 2017/05/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/ParticleEmitter3.h>
#include <Core/Utils/Macros.h>

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

	void ParticleEmitter3::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		if (m_onBeginUpdateCallback)
		{
			m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
		}

		OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
	}

	void ParticleEmitter3::OnSetTarget(const ParticleSystemData3Ptr& particles)
	{
		UNUSED_VARIABLE(particles);
	}

	void ParticleEmitter3::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
	{
		m_onBeginUpdateCallback = callback;
	}
}
