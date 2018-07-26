/*************************************************************************
> File Name: UpwindLevelSetSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Three-dimensional first-order upwind-based iterative level set solver.
> Created Time: 2017/09/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER3_H
#define CUBBYFLOW_UPWIND_LEVEL_SET_SOLVER3_H

#include <Core/Solver/LevelSet/IterativeLevelSetSolver3.h>

namespace CubbyFlow
{
	//! Three-dimensional first-order upwind-based iterative level set solver.
	class UpwindLevelSetSolver3 final : public IterativeLevelSetSolver3
	{
	public:
		//! Default constructor.
		UpwindLevelSetSolver3();

	protected:
		//! Computes the derivatives for given grid point.
		void GetDerivatives(
			ConstArrayAccessor3<double> grid,
			const Vector3D& gridSpacing,
			size_t i, size_t j, size_t k,
			std::array<double, 2>* dx, std::array<double, 2>* dy, std::array<double, 2>* dz) const override;
	};

	using UpwindLevelSetSolver3Ptr = std::shared_ptr<UpwindLevelSetSolver3>;
}

#endif