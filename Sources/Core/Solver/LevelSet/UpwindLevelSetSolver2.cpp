/*************************************************************************
> File Name: UpwindLevelSetSolver2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Two-dimensional first-order upwind-based iterative level set solver.
> Created Time: 2017/09/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Math/PDE.h>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver2.h>

namespace CubbyFlow
{
	UpwindLevelSetSolver2::UpwindLevelSetSolver2()
	{
		SetMaxCFL(0.5);
	}

	void UpwindLevelSetSolver2::GetDerivatives(
		ConstArrayAccessor2<double> grid,
		const Vector2D& gridSpacing,
		size_t i, size_t j,
		std::array<double, 2>* dx, std::array<double, 2>* dy) const
	{
		double d0[3];
		Size2 size = grid.size();

		const size_t im1 = (i < 1) ? 0 : i - 1;
		const size_t ip1 = std::min(i + 1, size.x - 1);
		const size_t jm1 = (j < 1) ? 0 : j - 1;
		const size_t jp1 = std::min(j + 1, size.y - 1);

		d0[0] = grid(im1, j);
		d0[1] = grid(i, j);
		d0[2] = grid(ip1, j);
		*dx = Upwind1(d0, gridSpacing.x);

		d0[0] = grid(i, jm1);
		d0[1] = grid(i, j);
		d0[2] = grid(i, jp1);
		*dy = Upwind1(d0, gridSpacing.y);
	}
}