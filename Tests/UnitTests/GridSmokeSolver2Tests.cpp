#include "gtest/gtest.h"

#include <Core/Solver/Grid/GridSmokeSolver2.hpp>

using namespace CubbyFlow;

TEST(GridSmokeSolver2, UpdateEmpty)
{
    GridSmokeSolver2 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}
