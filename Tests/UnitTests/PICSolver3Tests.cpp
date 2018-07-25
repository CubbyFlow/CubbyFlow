#include "pch.h"

#include <Core/Solver/Hybrid/PIC/PICSolver3.h>

using namespace CubbyFlow;

TEST(PICSolver3, UpdateEmpty)
{
	PICSolver3 solver;

	for (Frame frame; frame.index < 2; ++frame)
	{
		solver.Update(frame);
	}
}