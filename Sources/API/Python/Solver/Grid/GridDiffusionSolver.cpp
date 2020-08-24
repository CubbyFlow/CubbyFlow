// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridDiffusionSolver.hpp>
#include <Core/Solver/Grid/GridDiffusionSolver2.hpp>
#include <Core/Solver/Grid/GridDiffusionSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridDiffusionSolver2(pybind11::module& m)
{
    pybind11::class_<GridDiffusionSolver2, GridDiffusionSolver2Ptr>(
        static_cast<pybind11::handle>(m), "GridDiffusionSolver2",
        R"pbdoc(
			Abstract base class for 2-D grid-based diffusion equation solver.

			This class provides functions to solve the diffusion equation for different
			types of fields. The target equation can be written as
			\f$\frac{\partial f}{\partial t} = \mu\nabla^2 f\f$ where \f$\mu\f$ is
			the diffusion coefficient. The field \f$f\f$ can be either scalar or vector
			field.
		)pbdoc");
}

void AddGridDiffusionSolver3(pybind11::module& m)
{
    pybind11::class_<GridDiffusionSolver3, GridDiffusionSolver3Ptr>(
        static_cast<pybind11::handle>(m), "GridDiffusionSolver3",
        R"pbdoc(
			Abstract base class for 3-D grid-based diffusion equation solver.

			This class provides functions to solve the diffusion equation for different
			types of fields. The target equation can be written as
			\f$\frac{\partial f}{\partial t} = \mu\nabla^2 f\f$ where \f$\mu\f$ is
			the diffusion coefficient. The field \f$f\f$ can be either scalar or vector
			field.
		)pbdoc");
}