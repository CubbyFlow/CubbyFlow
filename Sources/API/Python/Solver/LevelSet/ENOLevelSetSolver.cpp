// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/LevelSet/ENOLevelSetSolver.hpp>
#include <Core/Solver/LevelSet/ENOLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/ENOLevelSetSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddENOLevelSetSolver2(pybind11::module& m)
{
    pybind11::class_<ENOLevelSetSolver2, ENOLevelSetSolver2Ptr,
                     IterativeLevelSetSolver2>(m, "ENOLevelSetSolver2",
                                               R"pbdoc(
			2-D third-order ENO-based iterative level set solver.
		)pbdoc");
}

void AddENOLevelSetSolver3(pybind11::module& m)
{
    pybind11::class_<ENOLevelSetSolver3, ENOLevelSetSolver3Ptr,
                     IterativeLevelSetSolver3>(m, "ENOLevelSetSolver3",
                                               R"pbdoc(
			3-D third-order ENO-based iterative level set solver.
		)pbdoc");
}