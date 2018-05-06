/*************************************************************************
> File Name: ParticleEmitter2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D particle emitter.
> Created Time: 2017/05/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_EMITTER2_H
#define CUBBYFLOW_PARTICLE_EMITTER2_H

#include <Core/Particle/ParticleSystemData2.h>

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
		using OnBeginUpdateCallback = std::function<void(ParticleEmitter2*, double, double)>;

		//! Default constructor.
		ParticleEmitter2();

		//! Destructor.
		virtual ~ParticleEmitter2();

		//! Updates the emitter state from \p currentTimeInSeconds to the following
		//! time-step.
		void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

		//! Returns the target particle system to emit.
		const ParticleSystemData2Ptr& GetTarget() const;

		//! Sets the target particle system to emit.
		void SetTarget(const ParticleSystemData2Ptr& particles);

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
		virtual void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) = 0;

	private:
		ParticleSystemData2Ptr m_particles;
		OnBeginUpdateCallback m_onBeginUpdateCallback;
	};

	//! Shared pointer for the ParticleEmitter2 type.
	using ParticleEmitter2Ptr = std::shared_ptr<ParticleEmitter2>;
}

#endif