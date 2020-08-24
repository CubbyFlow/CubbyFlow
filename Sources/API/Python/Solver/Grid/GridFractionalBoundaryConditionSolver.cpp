// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridFractionalBoundaryConditionSolver.hpp>
#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver2.hpp>
#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridFractionalBoundaryConditionSolver2(pybind11::module& m)
{
    pybind11::class_<GridFractionalBoundaryConditionSolver2,
                     GridFractionalBoundaryConditionSolver2Ptr,
                     GridBoundaryConditionSolver2>(
        static_cast<pybind11::handle>(m),
        "GridFractionalBoundaryConditionSolver2",
        R"pbdoc(
			Fractional 2-D boundary condition solver for grids.

			This class constrains the velocity field by projecting the flow to the
			signed-distance field representation of the collider. This implementation
			should pair up with GridFractionalSinglePhasePressureSolver2 to provide
			sub-grid resolutional velocity projection.
		)pbdoc")
        .def(pybind11::init<>())
        .def(
            "ConstrainVelocity",
            [](GridFractionalBoundaryConditionSolver2& instance,
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
            pybind11::arg("velocity"), pybind11::arg("extrapolationDepth") = 5)
        .def_property_readonly(
            "colliderSDF",
            &GridFractionalBoundaryConditionSolver2::GetColliderSDF,
            R"pbdoc(
			Signed distance field of the collider.
		)pbdoc")
        .def_property_readonly(
            "colliderVelocityField",
            &GridFractionalBoundaryConditionSolver2::GetColliderVelocityField,
            R"pbdoc(
			Velocity field of the collider.
		)pbdoc");
}

void AddGridFractionalBoundaryConditionSolver3(pybind11::module& m)
{
    pybind11::class_<GridFractionalBoundaryConditionSolver3,
                     GridFractionalBoundaryConditionSolver3Ptr,
                     GridBoundaryConditionSolver3>(
        static_cast<pybind11::handle>(m),
        "GridFractionalBoundaryConditionSolver3",
        R"pbdoc(
			Fractional 3-D boundary condition solver for grids.

			This class constrains the velocity field by projecting the flow to the
			signed-distance field representation of the collider. This implementation
			should pair up with GridFractionalSinglePhasePressureSolver3 to provide
			sub-grid resolutional velocity projection.
		)pbdoc")
        .def(pybind11::init<>())
        .def(
            "ConstrainVelocity",
            [](GridFractionalBoundaryConditionSolver3& instance,
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
            pybind11::arg("velocity"), pybind11::arg("extrapolationDepth") = 5)
        .def_property_readonly(
            "colliderSDF",
            &GridFractionalBoundaryConditionSolver3::GetColliderSDF,
            R"pbdoc(
			Signed distance field of the collider.
		)pbdoc")
        .def_property_readonly(
            "colliderVelocityField",
            &GridFractionalBoundaryConditionSolver3::GetColliderVelocityField,
            R"pbdoc(
			Velocity field of the collider.
		)pbdoc");
}