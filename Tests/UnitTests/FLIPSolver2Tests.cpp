#include "gtest/gtest.h"

#include <Core/Solver/Hybrid/FLIP/FLIPSolver2.hpp>

using namespace CubbyFlow;

TEST(FLIPSolver2, Empty)
{
    FLIPSolver2 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}

TEST(FLIPSolver2, PICBlendingFactor)
{
    FLIPSolver2 solver;

    solver.SetPICBlendingFactor(0.3);
    EXPECT_EQ(0.3, solver.GetPICBlendingFactor());

    solver.SetPICBlendingFactor(2.4);
    EXPECT_EQ(1.0, solver.GetPICBlendingFactor());

    solver.SetPICBlendingFactor(-0.9);
    EXPECT_EQ(0.0, solver.GetPICBlendingFactor());
}