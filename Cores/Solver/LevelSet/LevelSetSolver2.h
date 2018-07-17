/*************************************************************************
> File Name: LevelSetSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D level set solver.
> Created Time: 2017/08/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LEVEL_SET_SOLVER2_H
#define CUBBYFLOW_LEVEL_SET_SOLVER2_H

#include <Grid/CollocatedVectorGrid2.h>
#include <Grid/FaceCenteredGrid2.h>
#include <Grid/ScalarGrid2.h>

#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 2-D level set solver.
	class LevelSetSolver2
	{
	public:
		//! Default constructor.
		LevelSetSolver2();

		//! Default destructor.
		virtual ~LevelSetSolver2();

		//!
		//! Reinitializes given scalar field to signed-distance field.
		//!
		//! \param inputSDF Input signed-distance field which can be distorted.
		//! \param maxDistance Max range of reinitialization.
		//! \param outputSDF Output signed-distance field.
		//!
		virtual void Reinitialize(
			const ScalarGrid2& inputSDF,
			double maxDistance,
			ScalarGrid2* outputSDF) = 0;

		//!
		//! Extrapolates given scalar field from negative to positive SDF region.
		//!
		//! \param input Input scalar field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output scalar field.
		//!
		virtual void Extrapolate(
			const ScalarGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			ScalarGrid2* output) = 0;

		//!
		//! Extrapolates given collocated vector field from negative to positive SDF
		//! region.
		//!
		//! \param input Input collocated vector field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output collocated vector field.
		//!
		virtual void Extrapolate(
			const CollocatedVectorGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			CollocatedVectorGrid2* output) = 0;

		//!
		//! Extrapolates given face-centered vector field from negative to positive
		//! SDF region.
		//!
		//! \param input Input face-centered field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output face-centered vector field.
		//!
		virtual void Extrapolate(
			const FaceCenteredGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			FaceCenteredGrid2* output) = 0;
	};

	//! Shared pointer type for the LevelSetSolver2.
	using LevelSetSolver2Ptr = std::shared_ptr<LevelSetSolver2>;
}

#endif