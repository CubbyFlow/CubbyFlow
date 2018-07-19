/*************************************************************************
> File Name: ENOLevelSetSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Three-dimensional third-order ENO-based iterative level set solver.
> Created Time: 2017/08/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ENO_LEVEL_SET_SOLVER3_H
#define CUBBYFLOW_ENO_LEVEL_SET_SOLVER3_H

#include <Solver/LevelSet/IterativeLevelSetSolver3.h>

namespace CubbyFlow
{
	//! Three-dimensional third-order ENO-based iterative level set solver.
	class ENOLevelSetSolver3 final : public IterativeLevelSetSolver3
	{
	public:
		//! Default constructor.
		ENOLevelSetSolver3();

	protected:
		//! Computes the derivatives for given grid point.
		void GetDerivatives(
			ConstArrayAccessor3<double> grid,
			const Vector3D& gridSpacing,
			size_t i, size_t j, size_t k,
			std::array<double, 2>* dx, std::array<double, 2>* dy, std::array<double, 2>* dz) const override;
	};

	using ENOLevelSetSolver3Ptr = std::shared_ptr<ENOLevelSetSolver3>;
}

#endif