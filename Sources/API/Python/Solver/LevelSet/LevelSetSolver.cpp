/*************************************************************************
> File Name: LevelSetSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: LevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/LevelSet/LevelSetSolver.h>
#include <Core/Solver/LevelSet/LevelSetSolver2.h>
#include <Core/Solver/LevelSet/LevelSetSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddLevelSetSolver2(pybind11::module& m)
{
	pybind11::class_<LevelSetSolver2, LevelSetSolver2Ptr>(static_cast<pybind11::handle>(m), "LevelSetSolver2",
		R"pbdoc(
			Abstract base class for 2-D level set solver.
		)pbdoc");
}

void AddLevelSetSolver3(pybind11::module& m)
{
	pybind11::class_<LevelSetSolver3, LevelSetSolver3Ptr>(static_cast<pybind11::handle>(m), "LevelSetSolver3",
		R"pbdoc(
			Abstract base class for 3-D level set solver.
		)pbdoc");
}