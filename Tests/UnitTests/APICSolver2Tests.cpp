#include "pch.h"

#include <Core/Solver/Hybrid/APIC/APICSolver2.hpp>

using namespace CubbyFlow;

TEST(APICSolver2, UpdateEmpty)
{
    APICSolver2 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}