/*************************************************************************
> File Name: PICSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D Particle-in-Cell (PIC) implementation.
> Created Time: 2017/09/12
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PIC_SOLVER2_H
#define CUBBYFLOW_PIC_SOLVER2_H

#include <Emitter/ParticleEmitter2.h>
#include <Particle/ParticleSystemData2.h>
#include <Solver/Grid/GridFluidSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D Particle-in-Cell (PIC) implementation.
	//!
	//! This class implements 2-D Particle-in-Cell (PIC) method by inheriting
	//! GridFluidSolver2. Since it is a grid-particle hybrid method, the solver
	//! also has a particle system to track fluid particles.
	//!
	//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
	//!     ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
	//!
	class PICSolver2 : public GridFluidSolver2
	{
	public:
		class Builder;

		//! Default constructor.
		PICSolver2();

		//! Constructs solver with initial grid size.
		PICSolver2(
			const Size2& resolution,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin);

		//! Default destructor.
		virtual ~PICSolver2();

		//! Returns the signed-distance field of particles.
		ScalarGrid2Ptr GetSignedDistanceField() const;

		//! Returns the particle system data.
		const ParticleSystemData2Ptr& GetParticleSystemData() const;

		//! Returns the particle emitter.
		const ParticleEmitter2Ptr& GetParticleEmitter() const;

		//! Sets the particle emitter.
		void SetParticleEmitter(const ParticleEmitter2Ptr& newEmitter);

		//! Returns builder fox PICSolver2.
		static Builder GetBuilder();

	protected:
		Array2<char> m_uMarkers;
		Array2<char> m_vMarkers;

		//! Initializes the simulator.
		void OnInitialize() override;

		//! Invoked before a simulation time-step begins.
		void OnBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

		//! Computes the advection term of the fluid solver.
		void ComputeAdvection(double timeIntervalInSeconds) override;

		//! Returns the signed-distance field of the fluid.
		ScalarField2Ptr GetFluidSDF() const override;

		//! Transfers velocity field from particles to grids.
		virtual void TransferFromParticlesToGrids();

		//! Transfers velocity field from grids to particles.
		virtual void TransferFromGridsToParticles();

		//! Moves particles.
		virtual void MoveParticles(double timeIntervalInSeconds);

	private:
		size_t m_signedDistanceFieldID;
		ParticleSystemData2Ptr m_particles;
		ParticleEmitter2Ptr m_particleEmitter;

		void ExtrapolateVelocityToAir() const;

		void BuildSignedDistanceField();

		void UpdateParticleEmitter(double timeIntervalInSeconds) const;
	};

	//! Shared pointer type for the PICSolver2.
	using PICSolver2Ptr = std::shared_ptr<PICSolver2>;

	//!
	//! \brief Front-end to create PICSolver2 objects step by step.
	//!
	class PICSolver2::Builder final : public GridFluidSolverBuilderBase2<PICSolver2::Builder>
	{
	public:
		//! Builds PICSolver2.
		PICSolver2 Build() const;

		//! Builds shared pointer of PicSolver2 instance.
		PICSolver2Ptr MakeShared() const;
	};
}

#endif