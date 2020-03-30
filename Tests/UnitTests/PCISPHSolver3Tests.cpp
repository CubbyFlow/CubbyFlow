#include "pch.h"

#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.hpp>

using namespace CubbyFlow;

TEST(PCISPHSolver3, UpdateEmpty)
{
	// Empty solver test
	PCISPHSolver3 solver;
	Frame frame(0, 0.01);
	solver.Update(frame++);
	solver.Update(frame);
}

TEST(PCISPHSolver3, Parameters)
{
	PCISPHSolver3 solver;

	solver.SetMaxDensityErrorRatio(5.0);
	EXPECT_DOUBLE_EQ(5.0, solver.GetMaxDensityErrorRatio());

	solver.SetMaxDensityErrorRatio(-1.0);
	EXPECT_DOUBLE_EQ(0.0, solver.GetMaxDensityErrorRatio());

	solver.SetMaxNumberOfIterations(10);
	EXPECT_DOUBLE_EQ(10, solver.GetMaxNumberOfIterations());
}