// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/LevelSet/LevelSetSolver.hpp>
#include <Core/Solver/LevelSet/LevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/LevelSetSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddLevelSetSolver2(pybind11::module& m)
{
    pybind11::class_<LevelSetSolver2, LevelSetSolver2Ptr>(
        static_cast<pybind11::handle>(m), "LevelSetSolver2",
        R"pbdoc(
			Abstract base class for 2-D level set solver.
		)pbdoc");
}

void AddLevelSetSolver3(pybind11::module& m)
{
    pybind11::class_<LevelSetSolver3, LevelSetSolver3Ptr>(
        static_cast<pybind11::handle>(m), "LevelSetSolver3",
        R"pbdoc(
			Abstract base class for 3-D level set solver.
		)pbdoc");
}