#include "pch.h"

#include <Solver/Hybrid/FLIP/FLIPSolver3.h>

using namespace CubbyFlow;

TEST(FLIPSolver3, Empty)
{
	FLIPSolver3 solver;

	for (Frame frame; frame.index < 2; ++frame)
	{
		solver.Update(frame);
	}
}

TEST(FLIPSolver3, PICBlendingFactor)
{
	FLIPSolver3 solver;

	solver.SetPICBlendingFactor(0.3);
	EXPECT_EQ(0.3, solver.GetPICBlendingFactor());

	solver.SetPICBlendingFactor(2.4);
	EXPECT_EQ(1.0, solver.GetPICBlendingFactor());

	solver.SetPICBlendingFactor(-0.9);
	EXPECT_EQ(0.0, solver.GetPICBlendingFactor());
}