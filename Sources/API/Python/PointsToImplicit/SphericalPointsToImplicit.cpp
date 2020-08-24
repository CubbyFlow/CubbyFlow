// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/PointsToImplicit/SphericalPointsToImplicit.hpp>
#include <Core/PointsToImplicit/SphericalPointsToImplicit2.hpp>
#include <Core/PointsToImplicit/SphericalPointsToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSphericalPointsToImplicit2(pybind11::module& m)
{
    pybind11::class_<SphericalPointsToImplicit2, PointsToImplicit2,
                     SphericalPointsToImplicit2Ptr>(
        m, "SphericalPointsToImplicit2",
        R"pbdoc(
			2-D points-to-implicit converter based on simple sphere model.
		)pbdoc")
        .def(pybind11::init<double, bool>(),
             R"pbdoc(
			Constructs the converter with given sphere radius.

			Parameters
			----------
			- radius : Sphere radius.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("radius") = 1.0,
             pybind11::arg("isOutputSDF") = true);
}

void AddSphericalPointsToImplicit3(pybind11::module& m)
{
    pybind11::class_<SphericalPointsToImplicit3, PointsToImplicit3,
                     SphericalPointsToImplicit3Ptr>(
        m, "SphericalPointsToImplicit3",
        R"pbdoc(
			3-D points-to-implicit converter based on simple sphere model.
		)pbdoc")
        .def(pybind11::init<double, bool>(),
             R"pbdoc(
			Constructs the converter with given sphere radius.

			Parameters
			----------
			- radius : Sphere radius.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("radius") = 1.0,
             pybind11::arg("isOutputSDF") = true);
}