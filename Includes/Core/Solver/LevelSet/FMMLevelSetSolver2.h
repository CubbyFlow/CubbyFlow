/*************************************************************************
> File Name: FMMLevelSetSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Two-dimensional fast marching method (FMM) implementation.
> Created Time: 2017/09/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FMM_LEVEL_SET_SOLVER2_H
#define CUBBYFLOW_FMM_LEVEL_SET_SOLVER2_H

#include <Core/Solver/LevelSet/LevelSetSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief Two-dimensional fast marching method (FMM) implementation.
	//!
	//! This class implements 2-D FMM. First-order upwind-style differencing is used
	//! to solve the PDE.
	//!
	//! \see https://math.berkeley.edu/~sethian/2006/Explanations/fast_marching_explain.html
	//! \see Sethian, James A. "A fast marching level set method for monotonically
	//!     advancing fronts." Proceedings of the National Academy of Sciences 93.4
	//!     (1996): 1591-1595.
	//!
	class FMMLevelSetSolver2 final : public LevelSetSolver2
	{
	public:
		//! Default constructor.
		FMMLevelSetSolver2();

		//!
		//! Reinitializes given scalar field to signed-distance field.
		//!
		//! \param inputSDF Input signed-distance field which can be distorted.
		//! \param maxDistance Max range of reinitialization.
		//! \param outputSDF Output signed-distance field.
		//!
		void Reinitialize(
			const ScalarGrid2& inputSDF,
			double maxDistance,
			ScalarGrid2* outputSDF) override;

		//!
		//! Extrapolates given scalar field from negative to positive SDF region.
		//!
		//! \param input Input scalar field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output scalar field.
		//!
		void Extrapolate(
			const ScalarGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			ScalarGrid2* output) override;

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
			const CollocatedVectorGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			CollocatedVectorGrid2* output) override;

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
			const FaceCenteredGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			FaceCenteredGrid2* output) override;

	private:
		void Extrapolate(
			const ConstArrayAccessor2<double>& input,
			const ConstArrayAccessor2<double>& sdf,
			const Vector2D& gridSpacing,
			double maxDistance,
			ArrayAccessor2<double> output);
	};

	//! Shared pointer type for the FMMLevelSetSolver2.
	using FMMLevelSetSolver2Ptr = std::shared_ptr<FMMLevelSetSolver2>;
}

#endif