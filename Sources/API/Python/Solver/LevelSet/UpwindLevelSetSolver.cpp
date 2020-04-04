// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/LevelSet/UpwindLevelSetSolver.hpp>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/UpwindLevelSetSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddUpwindLevelSetSolver2(pybind11::module& m)
{
    pybind11::class_<UpwindLevelSetSolver2, UpwindLevelSetSolver2Ptr,
                     IterativeLevelSetSolver2>(static_cast<pybind11::handle>(m),
                                               "UpwindLevelSetSolver2",
                                               R"pbdoc(
			2-D first-order upwind-based iterative level set solver.
		)pbdoc");
}

void AddUpwindLevelSetSolver3(pybind11::module& m)
{
    pybind11::class_<UpwindLevelSetSolver3, UpwindLevelSetSolver3Ptr,
                     IterativeLevelSetSolver3>(static_cast<pybind11::handle>(m),
                                               "UpwindLevelSetSolver3",
                                               R"pbdoc(
			3-D first-order upwind-based iterative level set solver.
		)pbdoc");
}