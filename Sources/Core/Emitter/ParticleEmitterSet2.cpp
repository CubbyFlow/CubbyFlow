/*************************************************************************
> File Name: ParticleEmitterSet2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D particle-based emitter set.
> Created Time: 2017/05/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/ParticleEmitterSet2.h>

namespace CubbyFlow
{
	ParticleEmitterSet2::ParticleEmitterSet2()
	{
		// Do nothing
	}

	ParticleEmitterSet2::ParticleEmitterSet2(const std::vector<ParticleEmitter2Ptr>& emitters) :
		m_emitters(emitters)
	{
		// Do nothing
	}

	ParticleEmitterSet2::~ParticleEmitterSet2()
	{
		// Do nothing
	}

	void ParticleEmitterSet2::AddEmitter(const ParticleEmitter2Ptr& emitter)
	{
		m_emitters.push_back(emitter);
	}

	void ParticleEmitterSet2::OnSetTarget(const ParticleSystemData2Ptr& particles)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->SetTarget(particles);
		}
	}

	void ParticleEmitterSet2::OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->Update(currentTimeInSeconds, timeIntervalInSeconds);
		}
	}

	ParticleEmitterSet2::Builder ParticleEmitterSet2::GetBuilder()
	{
		return Builder();
	}
	
	ParticleEmitterSet2::Builder& ParticleEmitterSet2::Builder::WithEmitters(const std::vector<ParticleEmitter2Ptr>& emitters)
	{
		m_emitters = emitters;
		return *this;
	}

	ParticleEmitterSet2 ParticleEmitterSet2::Builder::Build() const
	{
		return ParticleEmitterSet2(m_emitters);
	}

	ParticleEmitterSet2Ptr ParticleEmitterSet2::Builder::MakeShared() const
	{
		return std::shared_ptr<ParticleEmitterSet2>(new ParticleEmitterSet2(m_emitters),
			[](ParticleEmitterSet2* obj)
		{
			delete obj;
		});
	}
}