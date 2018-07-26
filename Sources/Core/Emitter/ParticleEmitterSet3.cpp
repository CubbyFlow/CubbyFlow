/*************************************************************************
> File Name: ParticleEmitterSet3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D particle-based emitter set.
> Created Time: 2017/05/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Emitter/ParticleEmitterSet3.h>

namespace CubbyFlow
{
	ParticleEmitterSet3::ParticleEmitterSet3()
	{
		// Do nothing
	}

	ParticleEmitterSet3::ParticleEmitterSet3(const std::vector<ParticleEmitter3Ptr>& emitters) :
		m_emitters(emitters)
	{
		// Do nothing
	}

	ParticleEmitterSet3::~ParticleEmitterSet3()
	{
		// Do nothing
	}

	void ParticleEmitterSet3::AddEmitter(const ParticleEmitter3Ptr& emitter)
	{
		m_emitters.push_back(emitter);
	}

	void ParticleEmitterSet3::OnSetTarget(const ParticleSystemData3Ptr& particles)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->SetTarget(particles);
		}
	}

	void ParticleEmitterSet3::OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds)
	{
		for (auto& emitter : m_emitters)
		{
			emitter->Update(currentTimeInSeconds, timeIntervalInSeconds);
		}
	}

	ParticleEmitterSet3::Builder ParticleEmitterSet3::GetBuilder()
	{
		return Builder();
	}
	
	ParticleEmitterSet3::Builder& ParticleEmitterSet3::Builder::WithEmitters(const std::vector<ParticleEmitter3Ptr>& emitters)
	{
		m_emitters = emitters;
		return *this;
	}

	ParticleEmitterSet3 ParticleEmitterSet3::Builder::Build() const
	{
		return ParticleEmitterSet3(m_emitters);
	}

	ParticleEmitterSet3Ptr ParticleEmitterSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<ParticleEmitterSet3>(new ParticleEmitterSet3(m_emitters),
			[](ParticleEmitterSet3* obj)
		{
			delete obj;
		});
	}
}