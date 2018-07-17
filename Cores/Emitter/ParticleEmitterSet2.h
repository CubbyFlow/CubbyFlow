/*************************************************************************
> File Name: ParticleEmitterSet2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D particle-based emitter set.
> Created Time: 2017/05/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_EMITTER_SET2_H
#define CUBBYFLOW_PARTICLE_EMITTER_SET2_H

#include <Emitter/ParticleEmitter2.h>

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
		explicit ParticleEmitterSet2(const std::vector<ParticleEmitter2Ptr>& emitters);

		//! Destructor.
		virtual ~ParticleEmitterSet2();

		//! Adds sub-emitter.
		void AddEmitter(const ParticleEmitter2Ptr& emitter);

		//! Returns builder fox ParticleEmitterSet2.
		static Builder GetBuilder();

	private:
		std::vector<ParticleEmitter2Ptr> m_emitters;

		void OnSetTarget(const ParticleSystemData2Ptr& particles) override;

		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSecond) override;
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
}

#endif