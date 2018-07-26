/*************************************************************************
> File Name: UpwindLevelSetSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Two-dimensional first-order upwind-based iterative level set solver.
> Created Time: 2017/09/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER2_H
#define CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER2_H

#include <Core/Solver/LevelSet/IterativeLevelSetSolver2.h>

namespace CubbyFlow
{
	class UpwindLevelSetSolver2 final : public IterativeLevelSetSolver2
	{
	public:
		//! Default constructor.
		UpwindLevelSetSolver2();

	protected:
		//! Computes the derivatives for given grid point.
		void GetDerivatives(
			ConstArrayAccessor2<double> grid,
			const Vector2D& gridSpacing,
			size_t i, size_t j,
			std::array<double, 2>* dx, std::array<double, 2>* dy) const override;
	};

	using UpwindLevelSetSolver2Ptr = std::shared_ptr<UpwindLevelSetSolver2>;
}

#endif