/*************************************************************************
> File Name: LevelSetSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D level set solver.
> Created Time: 2017/08/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_LEVEL_SET_SOLVER3_H
#define CUBBYFLOW_LEVEL_SET_SOLVER3_H

#include <Core/Grid/CollocatedVectorGrid3.h>
#include <Core/Grid/FaceCenteredGrid3.h>
#include <Core/Grid/ScalarGrid3.h>

#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 3-D level set solver.
	class LevelSetSolver3
	{
	public:
		//! Default constructor.
		LevelSetSolver3();

		//! Default destructor.
		virtual ~LevelSetSolver3();

		//!
		//! Reinitializes given scalar field to signed-distance field.
		//!
		//! \param inputSDF Input signed-distance field which can be distorted.
		//! \param maxDistance Max range of reinitialization.
		//! \param outputSDF Output signed-distance field.
		//!
		virtual void Reinitialize(
			const ScalarGrid3& inputSDF,
			double maxDistance,
			ScalarGrid3* outputSDF) = 0;

		//!
		//! Extrapolates given scalar field from negative to positive SDF region.
		//!
		//! \param input Input scalar field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output scalar field.
		//!
		virtual void Extrapolate(
			const ScalarGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			ScalarGrid3* output) = 0;

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
			const CollocatedVectorGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			CollocatedVectorGrid3* output) = 0;

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
			const FaceCenteredGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			FaceCenteredGrid3* output) = 0;
	};

	//! Shared pointer type for the LevelSetSolver3.
	using LevelSetSolver3Ptr = std::shared_ptr<LevelSetSolver3>;
}

#endif