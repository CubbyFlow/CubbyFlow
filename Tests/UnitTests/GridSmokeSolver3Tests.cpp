#include "gtest/gtest.h"

#include <Core/Solver/Grid/GridSmokeSolver3.hpp>

using namespace CubbyFlow;

TEST(GridSmokeSolver3, UpdateEmpty)
{
    GridSmokeSolver3 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}
