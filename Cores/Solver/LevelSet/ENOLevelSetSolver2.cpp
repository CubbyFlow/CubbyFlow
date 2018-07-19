/*************************************************************************
> File Name: ENOLevelSetSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Two-dimensional third-order ENO-based iterative level set solver.
> Created Time: 2017/08/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Math/PDE.h>
#include <Solver/LevelSet/ENOLevelSetSolver2.h>

namespace CubbyFlow
{
	ENOLevelSetSolver2::ENOLevelSetSolver2()
	{
		SetMaxCFL(0.25);
	}

	void ENOLevelSetSolver2::GetDerivatives(
		ConstArrayAccessor2<double> grid,
		const Vector2D& gridSpacing,
		size_t i, size_t j,
		std::array<double, 2>* dx, std::array<double, 2>* dy) const
	{
		double d0[7];
		Size2 size = grid.size();

		const size_t im3 = (i < 3) ? 0 : i - 3;
		const size_t im2 = (i < 2) ? 0 : i - 2;
		const size_t im1 = (i < 1) ? 0 : i - 1;
		const size_t ip1 = std::min(i + 1, size.x - 1);
		const size_t ip2 = std::min(i + 2, size.x - 1);
		const size_t ip3 = std::min(i + 3, size.x - 1);
		const size_t jm3 = (j < 3) ? 0 : j - 3;
		const size_t jm2 = (j < 2) ? 0 : j - 2;
		const size_t jm1 = (j < 1) ? 0 : j - 1;
		const size_t jp1 = std::min(j + 1, size.y - 1);
		const size_t jp2 = std::min(j + 2, size.y - 1);
		const size_t jp3 = std::min(j + 3, size.y - 1);

		// 3rd order ENO differencing
		d0[0] = grid(im3, j);
		d0[1] = grid(im2, j);
		d0[2] = grid(im1, j);
		d0[3] = grid(i, j);
		d0[4] = grid(ip1, j);
		d0[5] = grid(ip2, j);
		d0[6] = grid(ip3, j);
		*dx = ENO3(d0, gridSpacing.x);

		d0[0] = grid(i, jm3);
		d0[1] = grid(i, jm2);
		d0[2] = grid(i, jm1);
		d0[3] = grid(i, j);
		d0[4] = grid(i, jp1);
		d0[5] = grid(i, jp2);
		d0[6] = grid(i, jp3);
		*dy = ENO3(d0, gridSpacing.y);
	}
}