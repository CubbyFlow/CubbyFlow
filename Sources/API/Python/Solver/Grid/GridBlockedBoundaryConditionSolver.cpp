// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridBlockedBoundaryConditionSolver.hpp>
#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver2.hpp>
#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridBlockedBoundaryConditionSolver2(pybind11::module& m)
{
    pybind11::class_<GridBlockedBoundaryConditionSolver2,
                     GridBlockedBoundaryConditionSolver2Ptr,
                     GridBoundaryConditionSolver2>(
        static_cast<pybind11::handle>(m), "GridBlockedBoundaryConditionSolver2",
        R"pbdoc(
			Blocked 2-D boundary condition solver for grids.

			This class constrains the velocity field by projecting the flow to the
			blocked representation of the collider. A collider is rasterized into voxels
			and each face of the collider voxels projects the velocity field onto its
			face. This implementation should pair up with GridSinglePhasePressureSolver2
			since the pressure solver assumes blocked boundary representation as well.
		)pbdoc")
        .def(pybind11::init<>())
        .def(
            "ConstrainVelocity",
            [](GridBlockedBoundaryConditionSolver2& instance,
               FaceCenteredGrid2Ptr velocity, unsigned int extrapolationDepth) {
                instance.ConstrainVelocity(velocity.get(), extrapolationDepth);
            },
            R"pbdoc(
			Constrains the velocity field to conform the collider boundary.

			Parameters
			----------
			- velocity : Input and output velocity grid.
			- extrapolationDepth : Number of inner-collider grid cells that velocity will get extrapolated.
		)pbdoc",
            pybind11::arg("velocity"), pybind11::arg("extrapolationDepth") = 5);
}

void AddGridBlockedBoundaryConditionSolver3(pybind11::module& m)
{
    pybind11::class_<GridBlockedBoundaryConditionSolver3,
                     GridBlockedBoundaryConditionSolver3Ptr,
                     GridBoundaryConditionSolver3>(
        static_cast<pybind11::handle>(m), "GridBlockedBoundaryConditionSolver3",
        R"pbdoc(
			Blocked 3-D boundary condition solver for grids.

			This class constrains the velocity field by projecting the flow to the
			blocked representation of the collider. A collider is rasterized into voxels
			and each face of the collider voxels projects the velocity field onto its
			face. This implementation should pair up with GridSinglePhasePressureSolver3
			since the pressure solver assumes blocked boundary representation as well.
		)pbdoc")
        .def(pybind11::init<>())
        .def(
            "ConstrainVelocity",
            [](GridBlockedBoundaryConditionSolver3& instance,
               FaceCenteredGrid3Ptr velocity, unsigned int extrapolationDepth) {
                instance.ConstrainVelocity(velocity.get(), extrapolationDepth);
            },
            R"pbdoc(
			Constrains the velocity field to conform the collider boundary.

			Parameters
			----------
			- velocity : Input and output velocity grid.
			- extrapolationDepth : Number of inner-collider grid cells that velocity will get extrapolated.
		)pbdoc",
            pybind11::arg("velocity"), pybind11::arg("extrapolationDepth") = 5);
}