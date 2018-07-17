#include "pch.h"

#include <Solver/Particle/PCISPH/PCISPHSolver2.h>

using namespace CubbyFlow;

TEST(PCISPHSolver2, UpdateEmpty)
{
	// Empty solver test
	PCISPHSolver2 solver;
	Frame frame(0, 0.01);
	solver.Update(frame++);
	solver.Update(frame);
}

TEST(PCISPHSolver2, Parameters)
{
	PCISPHSolver2 solver;

	solver.SetMaxDensityErrorRatio(5.0);
	EXPECT_DOUBLE_EQ(5.0, solver.GetMaxDensityErrorRatio());

	solver.SetMaxDensityErrorRatio(-1.0);
	EXPECT_DOUBLE_EQ(0.0, solver.GetMaxDensityErrorRatio());

	solver.SetMaxNumberOfIterations(10);
	EXPECT_DOUBLE_EQ(10, solver.GetMaxNumberOfIterations());
}