#include "pch.h"

#include <Core/Solver/Hybrid/PIC/PICSolver2.h>

using namespace CubbyFlow;

TEST(PICSolver2, UpdateEmpty)
{
	PICSolver2 solver;

	for (Frame frame; frame.index < 2; ++frame)
	{
		solver.Update(frame);
	}
}