#include "pch.h"

#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Solver/Grid/GridForwardEulerDiffusionSolver3.h>

using namespace CubbyFlow;

TEST(GridForwardEulerDiffusionSolver3, Solve)
{
	CellCenteredScalarGrid3 src(3, 3, 3, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
	CellCenteredScalarGrid3 dst(3, 3, 3, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);

	src(1, 1, 1) = 1.0;

	GridForwardEulerDiffusionSolver3 diffusionSolver;
	diffusionSolver.Solve(src, 1.0 / 12.0, 1.0, &dst);

	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(1, 1, 0));
	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(0, 1, 1));
	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(1, 0, 1));
	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(2, 1, 1));
	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(1, 2, 1));
	EXPECT_DOUBLE_EQ(1.0 / 12.0, dst(1, 1, 2));
	EXPECT_DOUBLE_EQ(1.0 / 2.0, dst(1, 1, 1));
}