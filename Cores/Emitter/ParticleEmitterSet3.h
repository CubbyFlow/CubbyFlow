/*************************************************************************
> File Name: ParticleEmitterSet3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D particle-based emitter set.
> Created Time: 2017/05/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_EMITTER_SET3_H
#define CUBBYFLOW_PARTICLE_EMITTER_SET3_H

#include <Emitter/ParticleEmitter3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D particle-based emitter set.
	//!
	class ParticleEmitterSet3 final : public ParticleEmitter3
	{
	public:
		class Builder;

		//! Constructs an emitter.
		ParticleEmitterSet3();

		//! Constructs an emitter with sub-emitters.
		explicit ParticleEmitterSet3(const std::vector<ParticleEmitter3Ptr>& emitters);

		//! Destructor.
		virtual ~ParticleEmitterSet3();

		//! Adds sub-emitter.
		void AddEmitter(const ParticleEmitter3Ptr& emitter);

		//! Returns builder fox ParticleEmitterSet3.
		static Builder GetBuilder();

	private:
		std::vector<ParticleEmitter3Ptr> m_emitters;

		void OnSetTarget(const ParticleSystemData3Ptr& particles) override;

		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSecond) override;
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
		Builder& WithEmitters(const std::vector<ParticleEmitter3Ptr>& emitters);

		//! Builds ParticleEmitterSet3.
		ParticleEmitterSet3 Build() const;

		//! Builds shared pointer of ParticleEmitterSet3 instance.
		ParticleEmitterSet3Ptr MakeShared() const;

	private:
		std::vector<ParticleEmitter3Ptr> m_emitters;
	};
}

#endif