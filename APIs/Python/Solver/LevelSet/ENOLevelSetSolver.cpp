/*************************************************************************
> File Name: ENOLevelSetSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ENOLevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Solver/LevelSet/ENOLevelSetSolver.h>
#include <Solver/LevelSet/ENOLevelSetSolver2.h>
#include <Solver/LevelSet/ENOLevelSetSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddENOLevelSetSolver2(pybind11::module& m)
{
	pybind11::class_<ENOLevelSetSolver2, ENOLevelSetSolver2Ptr, IterativeLevelSetSolver2>(m, "ENOLevelSetSolver2",
		R"pbdoc(
			2-D third-order ENO-based iterative level set solver.
		)pbdoc");
}

void AddENOLevelSetSolver3(pybind11::module& m)
{
	pybind11::class_<ENOLevelSetSolver3, ENOLevelSetSolver3Ptr, IterativeLevelSetSolver3>(m, "ENOLevelSetSolver3",
		R"pbdoc(
			3-D third-order ENO-based iterative level set solver.
		)pbdoc");
}