// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridBoundaryConditionSolver.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Solver/Grid/GridBoundaryConditionSolver2.hpp>
#include <Core/Solver/Grid/GridBoundaryConditionSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridBoundaryConditionSolver2(pybind11::module& m)
{
    pybind11::class_<GridBoundaryConditionSolver2,
                     GridBoundaryConditionSolver2Ptr>(
        static_cast<pybind11::handle>(m), "GridBoundaryConditionSolver2",
        R"pbdoc(
			Abstract base class for 2-D boundary condition solver for grids.

			This is a helper class to constrain the 2-D velocity field with given
			collider object. It also determines whether to open any domain boundaries.
			To control the friction level, tune the collider parameter.
		)pbdoc")
        .def_property_readonly("collider",
                               &GridBoundaryConditionSolver2::GetCollider)
        .def(
            "UpdateCollider",
            [](GridBoundaryConditionSolver2& instance,
               const Collider2Ptr& collider, pybind11::object gridSize,
               pybind11::object gridSpacing, pybind11::object gridOrigin) {
                instance.UpdateCollider(collider, ObjectToSize2(gridSize),
                                        ObjectToVector2D(gridSpacing),
                                        ObjectToVector2D(gridOrigin));
            },
            R"pbdoc(
			Applies new collider and build the internals.

			This function is called to apply new collider and build the internal
			cache. To provide a hint to the cache, info for the expected velocity
			grid that will be constrained is provided.

			Parameters
			----------
			- collider : New collider to apply.
			- gridSize : Size of the velocity grid to be constrained.
			- gridSpacing : Grid spacing of the velocity grid to be constrained.
			- gridOrigin : Origin of the velocity grid to be constrained.
		)pbdoc",
            pybind11::arg("collider"), pybind11::arg("gridSize"),
            pybind11::arg("gridSpacing"), pybind11::arg("gridOrigin"))
        .def_property(
            "closedDomainBoundaryFlag",
            &GridBoundaryConditionSolver2::GetClosedDomainBoundaryFlag,
            &GridBoundaryConditionSolver2::SetClosedDomainBoundaryFlag,
            R"pbdoc(
			Closed domain boundary flag.
		)pbdoc");
}

void AddGridBoundaryConditionSolver3(pybind11::module& m)
{
    pybind11::class_<GridBoundaryConditionSolver3,
                     GridBoundaryConditionSolver3Ptr>(
        static_cast<pybind11::handle>(m), "GridBoundaryConditionSolver3",
        R"pbdoc(
			Abstract base class for 3-D boundary condition solver for grids.

			This is a helper class to constrain the 3-D velocity field with given
			collider object. It also determines whether to open any domain boundaries.
			To control the friction level, tune the collider parameter.
		)pbdoc")
        .def_property_readonly("collider",
                               &GridBoundaryConditionSolver3::GetCollider)
        .def(
            "UpdateCollider",
            [](GridBoundaryConditionSolver3& instance,
               const Collider3Ptr& collider, pybind11::object gridSize,
               pybind11::object gridSpacing, pybind11::object gridOrigin) {
                instance.UpdateCollider(collider, ObjectToSize3(gridSize),
                                        ObjectToVector3D(gridSpacing),
                                        ObjectToVector3D(gridOrigin));
            },
            R"pbdoc(
			Applies new collider and build the internals.

			This function is called to apply new collider and build the internal
			cache. To provide a hint to the cache, info for the expected velocity
			grid that will be constrained is provided.

			Parameters
			----------
			- collider : New collider to apply.
			- gridSize : Size of the velocity grid to be constrained.
			- gridSpacing : Grid spacing of the velocity grid to be constrained.
			- gridOrigin : Origin of the velocity grid to be constrained.
		)pbdoc",
            pybind11::arg("collider"), pybind11::arg("gridSize"),
            pybind11::arg("gridSpacing"), pybind11::arg("gridOrigin"))
        .def_property(
            "closedDomainBoundaryFlag",
            &GridBoundaryConditionSolver3::GetClosedDomainBoundaryFlag,
            &GridBoundaryConditionSolver3::SetClosedDomainBoundaryFlag,
            R"pbdoc(
			Closed domain boundary flag.
		)pbdoc");
}