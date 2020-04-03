// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/FDM/FDMLinearSystemSolver.hpp>
#include <Core/Solver/FDM/FDMLinearSystemSolver2.hpp>
#include <Core/Solver/FDM/FDMLinearSystemSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFDMLinearSystemSolver2(pybind11::module& m)
{
    pybind11::class_<FDMLinearSystemSolver2, FDMLinearSystemSolver2Ptr>(
        static_cast<pybind11::handle>(m), "FDMLinearSystemSolver2",
        R"pbdoc(
			Abstract base class for 2-D finite difference-type linear system solver.
		)pbdoc");
}

void AddFDMLinearSystemSolver3(pybind11::module& m)
{
    pybind11::class_<FDMLinearSystemSolver3, FDMLinearSystemSolver3Ptr>(
        static_cast<pybind11::handle>(m), "FDMLinearSystemSolver3",
        R"pbdoc(
			Abstract base class for 3-D finite difference-type linear system solver.
		)pbdoc");
}