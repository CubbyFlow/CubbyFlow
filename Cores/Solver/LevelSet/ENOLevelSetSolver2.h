/*************************************************************************
> File Name: ENOLevelSetSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Two-dimensional third-order ENO-based iterative level set solver.
> Created Time: 2017/08/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ENO_LEVEL_SET_SOLVER2_H
#define CUBBYFLOW_ENO_LEVEL_SET_SOLVER2_H

#include <Solver/LevelSet/IterativeLevelSetSolver2.h>

namespace CubbyFlow
{
	//! Two-dimensional third-order ENO-based iterative level set solver.
	class ENOLevelSetSolver2 final : public IterativeLevelSetSolver2
	{
	public:
		//! Default constructor.
		ENOLevelSetSolver2();

	protected:
		//! Computes the derivatives for given grid point.
		void GetDerivatives(
			ConstArrayAccessor2<double> grid,
			const Vector2D& gridSpacing,
			size_t i, size_t j,
			std::array<double, 2>* dx, std::array<double, 2>* dy) const override;
	};

	using ENOLevelSetSolver2Ptr = std::shared_ptr<ENOLevelSetSolver2>;
}

#endif