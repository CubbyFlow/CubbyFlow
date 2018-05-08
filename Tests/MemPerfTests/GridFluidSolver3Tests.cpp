#include "MemPerfTestsUtils.h"

#include "gtest/gtest.h"

#include <Core/Animation/Frame.h>
#include <Core/Solver/Grid/GridFluidSolver3.h>

#include <iostream>

using namespace CubbyFlow;

TEST(GridFluidSolver3, Memory)
{
    const size_t n = 300;

    const size_t mem0 = GetCurrentRSS();

    auto solver = GridFluidSolver3::Builder()
        .WithResolution({ n, n, n })
        .MakeShared();

    const size_t mem1 = GetCurrentRSS();

    const auto msg1 = MakeReadableByteSize(mem1 - mem0);

    std::cout << "Start mem. usage: " << msg1.first << ' ' << msg1.second.c_str() << ".\n";

    solver->Update(Frame(1, 0.01));

    const size_t mem2 = GetCurrentRSS();

    const auto msg2 = MakeReadableByteSize(mem2 - mem0);

    std::cout << "Single update mem. usage: " << msg2.first << ' ' << msg2.second.c_str() << ".\n";
}