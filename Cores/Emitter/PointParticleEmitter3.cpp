/*************************************************************************
> File Name: PointParticleEmitter3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D point particle emitter.
> Created Time: 2017/07/22
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Emitter/PointParticleEmitter3.h>
#include <Matrix/Matrix3x3.h>
#include <Utils/Samplers.h>

namespace CubbyFlow
{
	PointParticleEmitter3::PointParticleEmitter3(
		const Vector3D& origin,
		const Vector3D& direction,
		double speed,
		double spreadAngleInDegrees,
		size_t maxNumOfNewParticlesPerSec,
		size_t maxNumOfParticles,
		uint32_t seed) :
		m_rng(seed),
		m_maxNumberOfNewParticlesPerSecond(maxNumOfNewParticlesPerSec),
		m_maxNumberOfParticles(maxNumOfParticles),
		m_origin(origin),
		m_direction(direction),
		m_speed(speed),
		m_spreadAngleInRadians(DegreesToRadians(spreadAngleInDegrees))
	{
		// Do nothing
	}

	size_t PointParticleEmitter3::GetMaxNumberOfNewParticlesPerSecond() const
	{
		return m_maxNumberOfNewParticlesPerSecond;
	}

	void PointParticleEmitter3::SetMaxNumberOfNewParticlesPerSecond(size_t rate)
	{
		m_maxNumberOfNewParticlesPerSecond = rate;
	}

	size_t PointParticleEmitter3::GetMaxNumberOfParticles() const
	{
		return m_maxNumberOfParticles;
	}

	void PointParticleEmitter3::SetMaxNumberOfParticles(size_t maxNumberOfParticles)
	{
		m_maxNumberOfParticles = maxNumberOfParticles;
	}

	void PointParticleEmitter3::OnUpdate(
		double currentTimeInSeconds,
		double timeIntervalInSeconds)
	{
		auto particles = GetTarget();

		if (particles == nullptr)
		{
			return;
		}

		if (m_numberOfEmittedParticles == 0)
		{
			m_firstFrameTimeInSeconds = currentTimeInSeconds;
		}

		double elapsedTimeInSeconds = currentTimeInSeconds - m_firstFrameTimeInSeconds;

		size_t newMaxTotalNumberOfEmittedParticles = static_cast<size_t>(std::ceil((elapsedTimeInSeconds + timeIntervalInSeconds) * m_maxNumberOfNewParticlesPerSecond));
		newMaxTotalNumberOfEmittedParticles = std::min(newMaxTotalNumberOfEmittedParticles, m_maxNumberOfParticles);
		size_t maxNumberOfNewParticles = newMaxTotalNumberOfEmittedParticles - m_numberOfEmittedParticles;

		if (maxNumberOfNewParticles > 0)
		{
			Array1<Vector3D> candidatePositions;
			Array1<Vector3D> candidateVelocities;
			Array1<Vector3D> newPositions;
			Array1<Vector3D> newVelocities;

			Emit(&candidatePositions, &candidateVelocities, maxNumberOfNewParticles);

			newPositions.Append(candidatePositions);
			newVelocities.Append(candidateVelocities);

			particles->AddParticles(newPositions, newVelocities);

			m_numberOfEmittedParticles += newPositions.size();
		}
	}

	void PointParticleEmitter3::Emit(
		Array1<Vector3D>* newPositions,
		Array1<Vector3D>* newVelocities,
		size_t maxNewNumberOfParticles)
	{
		for (size_t i = 0; i < maxNewNumberOfParticles; ++i)
		{
			Vector3D newDirection = UniformSampleCone(Random(), Random(), m_direction, m_spreadAngleInRadians);

			newPositions->Append(m_origin);
			newVelocities->Append(m_speed * newDirection);
		}
	}

	double PointParticleEmitter3::Random()
	{
		std::uniform_real_distribution<> d(0.0, 1.0);
		return d(m_rng);
	}

	PointParticleEmitter3::Builder PointParticleEmitter3::GetBuilder() 
	{
		return Builder();
	}
	
	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithOrigin(const Vector3D& origin)
	{
		m_origin = origin;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithDirection(const Vector3D& direction)
	{
		m_direction = direction;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithSpeed(double speed)
	{
		m_speed = speed;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithSpreadAngleInDegrees(double spreadAngleInDegrees)
	{
		m_spreadAngleInDegrees = spreadAngleInDegrees;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithMaxNumberOfNewParticlesPerSecond(size_t maxNumOfNewParticlesPerSec)
	{
		m_maxNumberOfNewParticlesPerSecond = maxNumOfNewParticlesPerSec;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithMaxNumberOfParticles(size_t maxNumberOfParticles)
	{
		m_maxNumberOfParticles = maxNumberOfParticles;
		return *this;
	}

	PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithRandomSeed(uint32_t seed)
	{
		m_seed = seed;
		return *this;
	}

	PointParticleEmitter3 PointParticleEmitter3::Builder::Build() const
	{
		return PointParticleEmitter3(
			m_origin,
			m_direction,
			m_speed,
			m_spreadAngleInDegrees,
			m_maxNumberOfNewParticlesPerSecond,
			m_maxNumberOfParticles,
			m_seed);
	}

	PointParticleEmitter3Ptr PointParticleEmitter3::Builder::MakeShared() const
	{
		return std::shared_ptr<PointParticleEmitter3>(
			new PointParticleEmitter3(
				m_origin,
				m_direction,
				m_speed,
				m_spreadAngleInDegrees,
				m_maxNumberOfNewParticlesPerSecond,
				m_maxNumberOfParticles,
				m_seed),
			[](PointParticleEmitter3* obj)
		{
			delete obj;
		});
	}
}