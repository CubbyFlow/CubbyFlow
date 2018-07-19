#include "pch.h"

#include <Solver/Hybrid/APIC/APICSolver3.h>

using namespace CubbyFlow;

TEST(APICSolver3, UpdateEmpty)
{
    APICSolver3 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}