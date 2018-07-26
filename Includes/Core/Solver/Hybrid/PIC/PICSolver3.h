/*************************************************************************
> File Name: PICSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D Particle-in-Cell (PIC) implementation.
> Created Time: 2017/09/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PIC_SOLVER3_H
#define CUBBYFLOW_PIC_SOLVER3_H

#include <Core/Emitter/ParticleEmitter3.h>
#include <Core/Particle/ParticleSystemData3.h>
#include <Core/Solver/Grid/GridFluidSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D Particle-in-Cell (PIC) implementation.
	//!
	//! This class implements 3-D Particle-in-Cell (PIC) method by inheriting
	//! GridFluidSolver3. Since it is a grid-particle hybrid method, the solver
	//! also has a particle system to track fluid particles.
	//!
	//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
	//!     ACM Transactions on Graphics (TOG). Vol. 34. No. 3. ACM, 3005.
	//!
	class PICSolver3 : public GridFluidSolver3
	{
	public:
		class Builder;

		//! Default constructor.
		PICSolver3();

		//! Constructs solver with initial grid size.
		PICSolver3(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin);

		//! Default destructor.
		virtual ~PICSolver3();

		//! Returns the signed-distance field of particles.
		ScalarGrid3Ptr GetSignedDistanceField() const;

		//! Returns the particle system data.
		const ParticleSystemData3Ptr& GetParticleSystemData() const;

		//! Returns the particle emitter.
		const ParticleEmitter3Ptr& GetParticleEmitter() const;

		//! Sets the particle emitter.
		void SetParticleEmitter(const ParticleEmitter3Ptr& newEmitter);

		//! Returns builder fox PICSolver3.
		static Builder GetBuilder();

	protected:
		Array3<char> m_uMarkers;
		Array3<char> m_vMarkers;
		Array3<char> m_wMarkers;

		//! Initializes the simulator.
		void OnInitialize() override;

		//! Invoked before a simulation time-step begins.
		void OnBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

		//! Computes the advection term of the fluid solver.
		void ComputeAdvection(double timeIntervalInSeconds) override;

		//! Returns the signed-distance field of the fluid.
		ScalarField3Ptr GetFluidSDF() const override;

		//! Transfers velocity field from particles to grids.
		virtual void TransferFromParticlesToGrids();

		//! Transfers velocity field from grids to particles.
		virtual void TransferFromGridsToParticles();

		//! Moves particles.
		virtual void MoveParticles(double timeIntervalInSeconds);

	private:
		size_t m_signedDistanceFieldID;
		ParticleSystemData3Ptr m_particles;
		ParticleEmitter3Ptr m_particleEmitter;

		void ExtrapolateVelocityToAir() const;

		void BuildSignedDistanceField();

		void UpdateParticleEmitter(double timeIntervalInSeconds) const;
	};

	//! Shared pointer type for the PICSolver3.
	using PICSolver3Ptr = std::shared_ptr<PICSolver3>;

	//!
	//! \brief Front-end to create PICSolver3 objects step by step.
	//!
	class PICSolver3::Builder final : public GridFluidSolverBuilderBase3<PICSolver3::Builder>
	{
	public:
		//! Builds PICSolver3.
		PICSolver3 Build() const;

		//! Builds shared pointer of PICSolver3 instance.
		PICSolver3Ptr MakeShared() const;
	};
}

#endif