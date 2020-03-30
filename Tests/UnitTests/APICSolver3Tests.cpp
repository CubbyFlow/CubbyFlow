#include "pch.h"

#include <Core/Solver/Hybrid/APIC/APICSolver3.hpp>

using namespace CubbyFlow;

TEST(APICSolver3, UpdateEmpty)
{
    APICSolver3 solver;

    for (Frame frame; frame.index < 2; ++frame)
    {
        solver.Update(frame);
    }
}