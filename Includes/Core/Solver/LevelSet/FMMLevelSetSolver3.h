/*************************************************************************
> File Name: FMMLevelSetSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Three-dimensional fast marching method (FMM) implementation.
> Created Time: 2017/09/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FMM_LEVEL_SET_SOLVER3_H
#define CUBBYFLOW_FMM_LEVEL_SET_SOLVER3_H

#include <Core/Solver/LevelSet/LevelSetSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief Three-dimensional fast marching method (FMM) implementation.
	//!
	//! This class implements 3-D FMM. First-order upwind-style differencing is used
	//! to solve the PDE.
	//!
	//! \see https://math.berkeley.edu/~sethian/2006/Explanations/fast_marching_explain.html
	//! \see Sethian, James A. "A fast marching level set method for monotonically
	//!     advancing fronts." Proceedings of the National Academy of Sciences 93.4
	//!     (1996): 1591-1595.
	//!
	class FMMLevelSetSolver3 final : public LevelSetSolver3
	{
	public:
		//! Default constructor.
		FMMLevelSetSolver3();

		//!
		//! Reinitializes given scalar field to signed-distance field.
		//!
		//! \param inputSDF Input signed-distance field which can be distorted.
		//! \param maxDistance Max range of reinitialization.
		//! \param outputSDF Output signed-distance field.
		//!
		void Reinitialize(
			const ScalarGrid3& inputSDF,
			double maxDistance,
			ScalarGrid3* outputSDF) override;

		//!
		//! Extrapolates given scalar field from negative to positive SDF region.
		//!
		//! \param input Input scalar field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output scalar field.
		//!
		void Extrapolate(
			const ScalarGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			ScalarGrid3* output) override;

		//!
		//! Extrapolates given collocated vector field from negative to positive SDF
		//! region.
		//!
		//! \param input Input collocated vector field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output collocated vector field.
		//!
		void Extrapolate(
			const CollocatedVectorGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			CollocatedVectorGrid3* output) override;

		//!
		//! Extrapolates given face-centered vector field from negative to positive
		//! SDF region.
		//!
		//! \param input Input face-centered field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output face-centered vector field.
		//!
		void Extrapolate(
			const FaceCenteredGrid3& input,
			const ScalarField3& sdf,
			double maxDistance,
			FaceCenteredGrid3* output) override;

	private:
		void Extrapolate(
			const ConstArrayAccessor3<double>& input,
			const ConstArrayAccessor3<double>& sdf,
			const Vector3D& gridSpacing,
			double maxDistance,
			ArrayAccessor3<double> output);
	};

	//! Shared pointer type for the FMMLevelSetSolver3.
	using FMMLevelSetSolver3Ptr = std::shared_ptr<FMMLevelSetSolver3>;
}

#endif