/*************************************************************************
> File Name: VolumeParticleEmitter2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D volumetric particle emitter.
> Created Time: 2017/06/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VOLUME_PARTICLE_EMITTER2_H
#define CUBBYFLOW_VOLUME_PARTICLE_EMITTER2_H

#include <Core/Emitter/ParticleEmitter2.h>
#include <Core/PointGenerator/PointGenerator2.h>
#include <Core/Surface/ImplicitSurface2.h>

#include <random>

namespace CubbyFlow
{
	//!
	//! \brief 2-D volumetric particle emitter.
	//!
	//! This class emits particles from volumetric geometry.
	//!
	class VolumeParticleEmitter2 final : public ParticleEmitter2
	{
	public:
		class Builder;

		//!
		//! Constructs an emitter that spawns particles from given implicit surface
		//! which defines the volumetric geometry. Provided bounding box limits
		//! the particle generation region.
		//!
		//! \param[in]  implicitSurface         The implicit surface.
		//! \param[in]  bounds                  The max region.
		//! \param[in]  spacing                 The spacing between particles.
		//! \param[in]  initialVel              The initial velocity.
		//! \param[in]  maxNumberOfParticles    The max number of particles to be
		//!                                     emitted.
		//! \param[in]  jitter                  The jitter amount between 0 and 1.
		//! \param[in]  isOneShot               Set true if particles are emitted
		//!                                     just once.
		//! \param[in]  allowOverlapping        True if particles can be overlapped.
		//! \param[in]  seed                    The random seed.
		//!
		VolumeParticleEmitter2(
			const ImplicitSurface2Ptr& implicitSurface,
			const BoundingBox2D& bounds,
			double spacing,
			const Vector2D& initialVel = Vector2D(),
			size_t maxNumberOfParticles = std::numeric_limits<size_t>::max(),
			double jitter = 0.0,
			bool isOneShot = true,
			bool allowOverlapping = false,
			uint32_t seed = 0);

		//!
		//! \brief      Sets the point generator.
		//!
		//! This function sets the point generator that defines the pattern of the
		//! point distribution within the volume.
		//!
		//! \param[in]  newPointsGen The new points generator.
		//!
		void SetPointGenerator(const PointGenerator2Ptr& newPointsGen);

		//! Returns jitter amount.
		double GetJitter() const;

		//! Sets jitter amount between 0 and 1.
		void SetJitter(double newJitter);

		//! Returns true if particles should be emitted just once.
		bool GetIsOneShot() const;

		//!
		//! \brief      Sets the flag to true if particles are emitted just once.
		//!
		//! If true is set, the emitter will generate particles only once even after
		//! multiple emit calls. If false, it will keep generating particles from
		//! the volumetric geometry. Default value is true.
		//!
		//! \param[in]  newValue True if particles should be emitted just once.
		//!
		void SetIsOneShot(bool newValue);

		//! Returns true if particles can be overlapped.
		bool GetAllowOverlapping() const;

		//!
		//! \brief      Sets the flag to true if particles can overlap each other.
		//!
		//! If true is set, the emitter will generate particles even if the new
		//! particles can find existing nearby particles within the particle
		//! spacing.
		//!
		//! \param[in]  newValue True if particles can be overlapped.
		//!
		void SetAllowOverlapping(bool newValue);

		//! Returns max number of particles to be emitted.
		size_t GetMaxNumberOfParticles() const;

		//! Sets the max number of particles to be emitted.
		void SetMaxNumberOfParticles(size_t newMaxNumberOfParticles);

		//! Returns the spacing between particles.
		double GetSpacing() const;

		//! Sets the spacing between particles.
		void SetSpacing(double newSpacing);

		//! Sets the initial velocity of the particles.
		Vector2D GetInitialVelocity() const;

		//! Returns the initial velocity of the particles.
		void SetInitialVelocity(const Vector2D& newInitialVel);

		//! Returns builder fox VolumeParticleEmitter2.
		static Builder GetBuilder();

	private:
		std::mt19937 m_rng;

		ImplicitSurface2Ptr m_implicitSurface;
		BoundingBox2D m_bounds;
		double m_spacing;
		Vector2D m_initialVel;
		PointGenerator2Ptr m_pointsGen;

		size_t m_maxNumberOfParticles = std::numeric_limits<size_t>::max();
		size_t m_numberOfEmittedParticles = 0;

		double m_jitter = 0.0;
		bool m_isOneShot = true;
		bool m_allowOverlapping = false;

		//!
		//! \brief      Emits particles to the particle system data.
		//!
		//! \param[in]  currentTimeInSeconds    Current simulation time.
		//! \param[in]  timeIntervalInSeconds   The time-step interval.
		//!
		void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;

		void Emit(const ParticleSystemData2Ptr& particles,
			Array1<Vector2D>* newPositions, Array1<Vector2D>* newVelocities);

		double Random();
	};

	//! Shared pointer for the VolumeParticleEmitter2 type.
	using VolumeParticleEmitter2Ptr = std::shared_ptr<VolumeParticleEmitter2>;
	
	//!
	//! \brief Front-end to create VolumeParticleEmitter2 objects step by step.
	//!
	class VolumeParticleEmitter2::Builder final
	{
	public:
		//! Returns builder with implicit surface defining volume shape.
		Builder& WithImplicitSurface(const ImplicitSurface2Ptr& implicitSurface);

		//! Returns builder with surface defining volume shape.
		Builder& WithSurface(const Surface2Ptr& surface);

		//! Returns builder with max region.
		Builder& WithMaxRegion(const BoundingBox2D& bounds);

		//! Returns builder with spacing.
		Builder& WithSpacing(double spacing);

		//! Returns builder with initial velocity.
		Builder& WithInitialVelocity(const Vector2D& initialVel);

		//! Returns builder with max number of particles.
		Builder& WithMaxNumberOfParticles(size_t maxNumberOfParticles);

		//! Returns builder with jitter amount.
		Builder& WithJitter(double jitter);

		//! Returns builder with one-shot flag.
		Builder& WithIsOneShot(bool isOneShot);

		//! Returns builder with overlapping flag.
		Builder& WithAllowOverlapping(bool allowOverlapping);

		//! Returns builder with random seed.
		Builder& WithRandomSeed(uint32_t seed);

		//! Builds VolumeParticleEmitter2.
		VolumeParticleEmitter2 Build() const;

		//! Builds shared pointer of VolumeParticleEmitter2 instance.
		VolumeParticleEmitter2Ptr MakeShared() const;

	private:
		ImplicitSurface2Ptr m_implicitSurface;
		bool m_isBoundSet = false;
		BoundingBox2D m_bounds;
		double m_spacing = 0.1;
		Vector2D m_initialVel{ 0, 0 };
		size_t m_maxNumberOfParticles = std::numeric_limits<size_t>::max();
		double m_jitter = 0.0;
		bool m_isOneShot = true;
		bool m_allowOverlapping = false;
		uint32_t m_seed = 0;
	};
}

#endif