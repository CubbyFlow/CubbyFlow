/*************************************************************************
> File Name: UpwindLevelSetSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: UpwindLevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/LevelSet/UpwindLevelSetSolver.h>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver2.h>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddUpwindLevelSetSolver2(pybind11::module& m)
{
	pybind11::class_<UpwindLevelSetSolver2, UpwindLevelSetSolver2Ptr, IterativeLevelSetSolver2>(static_cast<pybind11::handle>(m), "UpwindLevelSetSolver2",
		R"pbdoc(
			2-D first-order upwind-based iterative level set solver.
		)pbdoc");
}

void AddUpwindLevelSetSolver3(pybind11::module& m)
{
	pybind11::class_<UpwindLevelSetSolver3, UpwindLevelSetSolver3Ptr, IterativeLevelSetSolver3>(static_cast<pybind11::handle>(m), "UpwindLevelSetSolver3",
		R"pbdoc(
			3-D first-order upwind-based iterative level set solver.
		)pbdoc");
}