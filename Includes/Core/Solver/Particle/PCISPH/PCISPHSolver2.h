/*************************************************************************
> File Name: PCISPHSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D PCISPH solver.
> Created Time: 2017/06/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PCISPH_SOLVER2_H
#define CUBBYFLOW_PCISPH_SOLVER2_H

#include <Core/Solver/Particle/SPH/SPHSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D PCISPH solver.
	//!
	//! This class implements 2-D predictive-corrective SPH solver. The main
	//! pressure solver is based on Solenthaler and Pajarola's 2009 SIGGRAPH paper.
	//!
	//! \see Solenthaler and Pajarola, Predictive-corrective incompressible SPH,
	//!      ACM transactions on graphics (TOG). Vol. 28. No. 3. ACM, 2009.
	//!
	class PCISPHSolver2 : public SPHSolver2
	{
	public:
		class Builder;

		//! Constructs a solver with empty particle set.
		PCISPHSolver2();

		//! Constructs a solver with target density, spacing, and relative kernel radius.
		PCISPHSolver2(double targetDensity, double targetSpacing, double relativeKernelRadius);

		virtual ~PCISPHSolver2();

		//! Returns max allowed density error ratio.
		double GetMaxDensityErrorRatio() const;

		//!
		//! \brief Sets max allowed density error ratio.
		//!
		//! This function sets the max allowed density error ratio during the PCISPH
		//! iteration. Default is 0.01 (1%). The input value should be positive.
		//!
		void SetMaxDensityErrorRatio(double ratio);

		//! Returns max number of iterations.
		unsigned int GetMaxNumberOfIterations() const;

		//!
		//! \brief Sets max number of PCISPH iterations.
		//!
		//! This function sets the max number of PCISPH iterations. Default is 5.
		//!
		void SetMaxNumberOfIterations(unsigned int n);

		//! Returns builder fox PCISPHSolver2.
		static Builder GetBuilder();

	protected:
		//! Accumulates the pressure force to the forces array in the particle system.
		void AccumulatePressureForce(double timeIntervalInSeconds) override;

		//! Performs pre-processing step before the simulation.
		void OnBeginAdvanceTimeStep(double timeStepInSeconds) override;

	private:
		double m_maxDensityErrorRatio = 0.01;
		unsigned int m_maxNumberOfIterations = 5;

		ParticleSystemData2::VectorData m_tempPositions;
		ParticleSystemData2::VectorData m_tempVelocities;
		ParticleSystemData2::VectorData m_pressureForces;
		ParticleSystemData2::ScalarData m_densityErrors;

		double ComputeDelta(double timeStepInSeconds) const;
		double ComputeBeta(double timeStepInSeconds) const;
	};

	//! Shared pointer type for the PCISPHSolver2.
	using PCISPHSolver2Ptr = std::shared_ptr<PCISPHSolver2>;

	//!
	//! \brief Front-end to create PCISPHSolver2 objects step by step.
	//!
	class PCISPHSolver2::Builder final : public SPHSolverBuilderBase2<PCISPHSolver2::Builder>
	{
	public:
		//! Builds PCISPHSolver2.
		PCISPHSolver2 Build() const;

		//! Builds shared pointer of PCISPHSolver2 instance.
		PCISPHSolver2Ptr MakeShared() const;
	};
}

#endif